; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "ChainSaw"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "Scriptor25"
#define MyAppURL "https://github.com/Scriptor25/ChainSaw"
#define MyAppExeName "csaw.exe"
#define MyAppAssocNameSource MyAppName + " Source File"
#define MyAppAssocNameHeader MyAppName + " Header File"
#define MyAppAssocExtSource ".csaw"
#define MyAppAssocExtHeader ".csh"
#define MyAppAssocKeySource StringChange(MyAppAssocNameSource, " ", "") + MyAppAssocExtSource
#define MyAppAssocKeyHeader StringChange(MyAppAssocNameHeader, " ", "") + MyAppAssocExtHeader
#define MyAppIcoFile "logo.ico"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{FEC84C9C-DCB2-458A-971E-061AE29CE931}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
ChangesAssociations=no
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=auto
LicenseFile=..\LICENSE
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog
OutputBaseFilename=ChainSawSetup
SetupIconFile={#MyAppIcoFile}
Compression=lzma
SolidCompression=yes
WizardStyle=modern
AllowCancelDuringInstall=yes
AppContact=f.schreiber.2006@proton.me
ChangesEnvironment=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "..\install\bin\*";     DestDir: "{app}\bin";     Components: mainexe; Flags: ignoreversion recursesubdirs
Source: "..\install\include\*"; DestDir: "{app}\include"; Components: headers; Flags: ignoreversion recursesubdirs
Source: "..\install\lib\*";     DestDir: "{app}\lib";     Components: libs;    Flags: ignoreversion recursesubdirs
Source: "{#MyAppIcoFile}";      DestDir: "{app}";                              Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Registry]
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExtSource}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKeySource}"; ValueData: ""; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExtHeader}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKeyHeader}"; ValueData: ""; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKeySource}"; ValueType: string; ValueName: ""; ValueData: "{#MyAppAssocNameSource}"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKeyHeader}"; ValueType: string; ValueName: ""; ValueData: "{#MyAppAssocNameHeader}"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKeySource}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\logo.ico"
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKeyHeader}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\logo.ico"
Root: HKA; Subkey: "Software\Classes\Applications\{#MyAppExeName}\SupportedTypes"; ValueType: string; ValueName: ".csaw"; ValueData: ""
Root: HKA; Subkey: "Software\Classes\Applications\{#MyAppExeName}\SupportedTypes"; ValueType: string; ValueName: ".csh"; ValueData: ""

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\bin\{#MyAppExeName}"; IconFilename: "{app}\logo.ico"
Name: "{group}\Uninstall {#MyAppName}"; Filename: "{uninstallexe}"

[Types]
Name: "full"; Description: "Full Installation"
Name: "compact"; Description: "Compact Installation"
Name: "custom"; Description: "Custom Installation"; Flags: iscustom

[Components]
Name: "mainexe"; Description: "Main EXE File";               Types: full compact custom; Flags: fixed
Name: "headers"; Description: "STD Header Files";            Types: full custom
Name: "libs";    Description: "Additional Library Binaries"; Types: full custom

[Tasks]
Name: envPath; Description: "Add ChainSaw to the PATH" 

[Code]
const EnvironmentKey = 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment';

procedure EnvAddPath(Path: string);
var
    Paths: string;
begin
    { Retrieve current path (use empty string if entry not exists) }
    if not RegQueryStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths)
    then Paths := '';

    { Skip if string already found in path }
    if Pos(';' + Uppercase(Path) + ';', ';' + Uppercase(Paths) + ';') > 0 then exit;

    { App string to the end of the path variable }
    Paths := Paths + ';'+ Path +';'

    { Overwrite (or create if missing) path environment variable }
    if RegWriteStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths)
    then Log(Format('The [%s] added to PATH: [%s]', [Path, Paths]))
    else Log(Format('Error while adding the [%s] to PATH: [%s]', [Path, Paths]));
end;

procedure EnvRemovePath(Path: string);
var
    Paths: string;
    P: Integer;
begin
    { Skip if registry entry not exists }
    if not RegQueryStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths) then
        exit;

    { Skip if string not found in path }
    P := Pos(';' + Uppercase(Path) + ';', ';' + Uppercase(Paths) + ';');
    if P = 0 then exit;

    { Update path variable }
    Delete(Paths, P - 1, Length(Path) + 1);

    { Overwrite path environment variable }
    if RegWriteStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths)
    then Log(Format('The [%s] removed from PATH: [%s]', [Path, Paths]))
    else Log(Format('Error while removing the [%s] from PATH: [%s]', [Path, Paths]));
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
    if (CurStep = ssPostInstall) and WizardIsTaskSelected('envPath')
    then EnvAddPath(ExpandConstant('{app}') + '\bin');
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
    if CurUninstallStep = usPostUninstall
    then EnvRemovePath(ExpandConstant('{app}') + '\bin');
end;