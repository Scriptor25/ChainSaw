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
#define MyAppIcoFile "C:\Users\FelixSchreiber\Documents\Projects\C++\ChainSaw\installer\logo.ico"

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
DisableProgramGroupPage=yes
LicenseFile=C:\Users\FelixSchreiber\Documents\Projects\C++\ChainSaw\LICENSE
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog
OutputBaseFilename=ChainSawSetup
SetupIconFile={#MyAppIcoFile}
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "C:\Users\FelixSchreiber\Documents\Projects\C++\ChainSaw\out\install\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs
Source: "{#MyAppIcoFile}"; DestDir: "{app}"; Flags: ignoreversion
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
Name: "{group}\{#MyAppName}"; Filename: "{app}\bin\{#MyAppExeName}"

[Run]
Filename: "{app}\bin\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

