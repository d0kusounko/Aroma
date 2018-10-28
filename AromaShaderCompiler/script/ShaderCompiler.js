//===========================================================================
//!
//!	@file		ShaderCompiler.js
//!	@brief		Aroma �V�F�[�_�[�R���p�C��.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author 	d0
//!
//!	@usage		Usage : ShaderCompile.js Platform Configuration ShaderCompilerPath ShaderProfile ShaderSource OutputFile
//!				[platform] ... x86/x64
//!
//===========================================================================

var DebugOutput = false;

// ������`.
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

// �G���[�R�[�h.
var ErrorCode =
{
	kNone : 0,
	kUsage : 1,
	kError : 2
};

// �I�u�W�F�N�g����.
var g_Fso	= new ActiveXObject( "Scripting.FileSystemObject" );	//!< �t�@�C��IO.
var g_Shell	= new ActiveXObject( "WScript.Shell" );					//!< �V�F��.
var g_Env	= g_Shell.Environment( "PROCESS" );						//!< �v���Z�X���ϐ�.

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
	NOT_EXIST	: 1,	//!< �V�K�쐬���A�t�@�C�������݂���ꍇ�͏㏑�����Ȃ�.
	OVER_WRITE	: 2		//!< �V�K�쐬���A�t�@�C�������݂���ꍇ�͏㏑������.
};

// �����o��.
function Print( format )
{
	WScript.Echo( format );
}

// Usage�\��.
function UsagePrint()
{
	Print( "Usage : ShaderCompile.js Platform Configuration ShaderCompilerPath ShaderProfile ShaderSource OutputFile" );
	Print( "<platform>: x86/x64" );
}

// -----------------------------------------

// �����`�F�b�N.
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
		// PATH�ɒǉ�.
		var path = g_Env.item( "PATH" );
		g_Env.item( "PATH" ) = shaderCompilerPath + ";" + path;
	}

	// 4: ShaderProfile
	shaderProfile		= arg[ ArgEnum.kShaderProfile ];

	// 5: ShaderSource
	shaderSource		= arg[ ArgEnum.kShaderSource ];
	shaderSource		= g_Fso.GetAbsolutePathName( shaderSource );	// �t���p�X��.

	// 6: OutputFile
	outputFile			= arg[ ArgEnum.kOutputFile ];
	outputFile			= g_Fso.GetAbsolutePathName( outputFile );	// �t���p�X��.

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

	// �R���p�C�����s.
	errorCode = Compile( platform, configuration, shaderCompilerPath, shaderSource, outputFile, shaderProfile );
	WScript.Quit( errorCode );
}
else
{
	UsagePrint();
	WScript.Quit( ErrorCode.kUsage );
}

// �R���p�C�����s.
function Compile( platform, configuration, shaderCompilerPath, shaderSource, outputFile, shaderProfile )
{
	var errorCode			= ErrorCode.kNone;

	Print( "[Aroma Shader Compiling] " + shaderSource );

	// TODO: configuration ���O���t�B�b�N�XAPI��Debug,Release�Ȃǂ̐ݒ�𕪗����Ă����Ɣ���.
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

// fxc.exe�ł̃V�F�[�_�[�R���p�C��.
function CompileFXC( platform, configuration, shaderCompilerPath, shaderSource, outputFile, shaderProfile )
{
	var errorCode	=  ErrorCode.kNone;
	var shaderArg	= "";

	// �R���p�C���p�����쐬.
	{
		// �G���g���[�|�C���g.
		shaderArg += " /E main";

		// �V�F�[�_�[�v���t�@�C��.
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

		// �o�̓w�b�_�[.
		shaderArg	+= " /Fh " + "\"" + outputFile + "\"";

		// ���̓V�F�[�_�\�[�X.
		shaderArg	+= " " + "\"" + shaderSource + "\"";
	}

	if( DebugOutput )
	{
		Print( "Shader Compiler Args = " + shaderArg );
	}

	// �R���p�C�����s.
	var exec = g_Shell.Exec( "fxc.exe" + shaderArg );
	while( exec.Status == 0 )
	{
		WScript.Sleep( 10 );
	}

	// �R���p�C���o�͓��e�\��.
	if( DebugOutput )
	{
		var stdOut = exec.StdOut.ReadAll();
		if( stdOut ) Print( stdOut() );
	}
	var stdErr = exec.StdErr.ReadAll();
	if( stdErr ) Print( stdErr );

	// �G���[�`�F�b�N.
	if( exec.ExitCode != 0 )
	{
		errorCode =  ErrorCode.kError;
	}

	return errorCode;
}

// �w��t�H���_�[�܂ł̑S�K�w�f�B���N�g�����쐬.
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
