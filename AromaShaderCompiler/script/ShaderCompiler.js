//===========================================================================
//!
//!	@file		ShaderCompiler.js
//!	@brief		Aroma シェーダーコンパイラ.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author 	d0
//!
//!	@usage		Usage : ShaderCompile.js Platform Configuration ShaderCompilerPath ShaderProfile ShaderSource OutputFile
//!				[platform] ... x86/x64
//!
//===========================================================================

var DebugOutput = false;

// 引数定義.
var ArgEnum =
{
	kPlatform : 0,
	kConfiguration : 1,
	kShaderCompilerPath : 2,
	kShaderProfile : 3,
	kShaderSource : 4,
	kOutputFile : 5,
	kNum : 6
};

// エラーコード.
var ErrorCode =
{
	kNone : 0,
	kUsage : 1,
	kError : 2
};

// オブジェクト生成.
var g_Fso	= new ActiveXObject( "Scripting.FileSystemObject" );	//!< ファイルIO.
var g_Shell	= new ActiveXObject( "WScript.Shell" );					//!< シェル.
var g_Env	= g_Shell.Environment( "PROCESS" );						//!< プロセス環境変数.

var adStreamMode	=
{
	READ		: 1,
	WRITE		: 2,
	READ_WRITE	: 3
};

var adStreamType	=
{
	BINARY		: 1,
	TEXT		: 2
};

var adSaveCreateMode	=
{
	NOT_EXIST	: 1,	//!< 新規作成し、ファイルが存在する場合は上書きしない.
	OVER_WRITE	: 2		//!< 新規作成し、ファイルが存在する場合は上書きする.
};

// 文字出力.
function Print( format )
{
	WScript.Echo( format );
}

// Usage表示.
function UsagePrint()
{
	Print( "Usage : ShaderCompile.js Platform Configuration ShaderCompilerPath ShaderProfile ShaderSource OutputFile" );
	Print( "<platform>: x86/x64" );
}

// -----------------------------------------

// 引数チェック.
if( WScript.Arguments.length == ArgEnum.kNum )
{
	var errorCode			= ErrorCode.kNone;
	var arg					= [];
	var platform			= "";
	var configuration		= "";
	var shaderCompilerPath	= "";
	var shaderSource		= "";
	var outputFile			= "";
	var shaderProfile		= "";
	for( var i = 0; i < WScript.Arguments.length; i++ )
	{
		arg.push( WScript.Arguments( i ) );
	}

	// 1: Platform
	platform			= arg[ ArgEnum.kPlatform ];

	// 2: Configuration
	configuration		= arg[ ArgEnum.kConfiguration ];

	// 3: ShaderCompilerPath
	shaderCompilerPath	= arg[ ArgEnum.kShaderCompilerPath ];
	{
		// PATHに追加.
		var path = g_Env.item( "PATH" );
		g_Env.item( "PATH" ) = shaderCompilerPath + ";" + path;
	}

	// 4: ShaderProfile
	shaderProfile		= arg[ ArgEnum.kShaderProfile ];

	// 5: ShaderSource
	shaderSource		= arg[ ArgEnum.kShaderSource ];
	shaderSource		= g_Fso.GetAbsolutePathName( shaderSource );	// フルパス化.

	// 6: OutputFile
	outputFile			= arg[ ArgEnum.kOutputFile ];
	outputFile			= g_Fso.GetAbsolutePathName( outputFile );	// フルパス化.

	if( DebugOutput )
	{
		Print( "platform = " + platform );
		Print( "configuration = " + configuration );
		Print( "shaderCompilerPath = " + shaderCompilerPath );
		Print( "shaderSource = " + shaderSource );
		Print( "outputFile = " + outputFile );
		Print( "shaderProfile = " + shaderProfile );
		Print( "PATH = " + g_Env.item( "PATH" ) );
	}

	// コンパイル実行.
	errorCode = Compile( platform, configuration, shaderCompilerPath, shaderSource, outputFile, shaderProfile );
	WScript.Quit( errorCode );
}
else
{
	UsagePrint();
	WScript.Quit( ErrorCode.kUsage );
}

// コンパイル実行.
function Compile( platform, configuration, shaderCompilerPath, shaderSource, outputFile, shaderProfile )
{
	var errorCode			= ErrorCode.kNone;

	Print( "[Aroma Shader Compiling] " + shaderSource );

	// TODO: configuration をグラフィックスAPIとDebug,Releaseなどの設定を分離してちゃんと判別.
	switch( platform )
	{
		// Windows.
		case "x86":
		case "x64":
		{
			errorCode = CompileFXC( platform, configuration, shaderCompilerPath, shaderSource, outputFile, shaderProfile );
			break;
		}
	}

	return errorCode;
}

// fxc.exeでのシェーダーコンパイル.
function CompileFXC( platform, configuration, shaderCompilerPath, shaderSource, outputFile, shaderProfile )
{
	var errorCode	=  ErrorCode.kNone;
	var shaderArg	= "";

	// コンパイラ用引数作成.
	{
		// エントリーポイント.
		shaderArg += " /E main";

		// シェーダープロファイル.
		var shaderArg	= " /T ";
		switch( shaderProfile )
		{
			case "vs":
				shaderArg	+= "vs_5_0";
				break;

			case "ps":
				shaderArg	+= "ps_5_0";
				break;

			case "gs":
				shaderArg	+= "gs_5_0";
				break;

			case "cs":
				shaderArg	+= "cs_5_0";
				break;

			default:
				errorCode = ErrorCode.kError;
				Print( "[Aroma Shader Compiling] Invalid shader profile." );
				break;
		}

		// 出力ヘッダー.
		shaderArg	+= " /Fh " + "\"" + outputFile + "\"";

		// 入力シェーダソース.
		shaderArg	+= " " + "\"" + shaderSource + "\"";
	}

	if( DebugOutput )
	{
		Print( "Shader Compiler Args = " + shaderArg );
	}

	// コンパイラ実行.
	var exec = g_Shell.Exec( "fxc.exe" + shaderArg );
	while( exec.Status == 0 )
	{
		WScript.Sleep( 10 );
	}

	// コンパイラ出力内容表示.
	if( DebugOutput )
	{
		var stdOut = exec.StdOut.ReadAll();
		if( stdOut ) Print( stdOut() );
	}
	var stdErr = exec.StdErr.ReadAll();
	if( stdErr ) Print( stdErr );

	// エラーチェック.
	if( exec.ExitCode != 0 )
	{
		errorCode =  ErrorCode.kError;
	}

	return errorCode;
}

// 指定フォルダーまでの全階層ディレクトリを作成.
function CreateFolders( dir )
{
	if( !g_Fso.FolderExists( dir ) )
	{
		parent_dir	= g_Fso.GetParentFolderName( dir );
		if( !g_Fso.FolderExists( parent_dir ) )
		{
			CreateFolders( parent_dir );
		}
		g_Fso.CreateFolder( dir );
	}
}
