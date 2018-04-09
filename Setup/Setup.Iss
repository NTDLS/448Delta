[Setup]
;-- Main Setup Information
 AppName                = 448Delta Encryption
 AppVerName             = 448Delta Encryption 1.0.3.4 (Beta)
 AppCopyright           = Copyright © 1995-2003 NetWorkDLS.
 DefaultDirName         = {pf}\NetWorkDLS\448Delta
 DefaultGroupName       = NetWorkDLS\448Delta
 UninstallDisplayIcon   = {app}\448Delta.Exe
 UninstallStyle         = Modern
 WizardImageFile        = Resources\LgSetup.bmp
 WizardSmallImageFile   = Resources\SmSetup.bmp
 PrivilegesRequired     = PowerUser
 Uninstallable          = Yes
 AppMutex               = 448Delta Encryption
 LicenseFile            = Resources\EULA.txt
 Compression            = ZIP/9
 ChangesAssociations    = Yes
 OutputBaseFilename     = 448Delta

;-- Windows 2000 & XP (Support Dialog)
 AppPublisher    = NetWorkDLS
 AppPublisherURL = http://www.NetWorkDLS.com/
 AppUpdatesURL   = http://www.NetWorkDLS.com/
 AppVersion      = 1.0.3.4

[Types]
 Name: "Full";    Description: "Full installation"
 Name: "Compact"; Description: "Compact installation"
 Name: "Custom";  Description: "Custom installation"; Flags: iscustom

[Components]
 Name: "Main";          Description: "Main Files";                      Types: Full Compact Custom; Flags: fixed;
 Name: "DLL";           Description: "448Delta DLL's";                  Types: Full Compact Custom; Flags: fixed;
 Name: "Help";          Description: "Help File";                       Types: Full Custom;
 Name: "SelfExe";       Description: "Self Extract Support";            Types: Full;
 Name: "Thread";        Description: "Advanced Threading Support";      Types: Full;

[Tasks]
 Name: DesktopIcon; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:";

[Files]
 Source: "..\Debug\448Delta.Exe";                     DestDir: "{app}"; Flags: IgnoreVersion;  Components: Main;
;Source: "..\Release\448Delta.Exe";                   DestDir: "{app}"; Flags: IgnoreVersion;  Components: Main;
 Source: "Resources\EULA.txt";                        DestDir: "{app}"; Flags: IgnoreVersion;  Components: Main;
 Source: "Help\448Delta.Chm";                         DestDir: "{app}"; Flags: IgnoreVersion;  Components: Help;
 Source: "SelfAppend\Release\SelfAppend.Exe";         DestDir: "{app}"; Flags: IgnoreVersion;  Components: SelfExe;
 Source: "SelfExtract\Release\SelfExtract.Exe";       DestDir: "{app}"; Flags: IgnoreVersion;  Components: SelfExe;
 Source: "448Priority\Release\448Priority.Exe";       DestDir: "{app}"; Flags: IgnoreVersion;  Components: Thread;
 Source: "ShellExt\Release\448Shell.Dll";             DestDir: "{sys}"; Flags: RestartReplace; Components: DLL;
 Source: "Libraries\NSWFL.Dll";                       DestDir: "{app}"; Flags: IgnoreVersion;  Components: DLL;
 Source: "Libraries\NASCCL.Dll";                      DestDir: "{app}"; Flags: IgnoreVersion;  Components: DLL;

[Icons]
 Name: "{group}\448Delta Encryption";          Filename: "{app}\448Delta.Exe";    Components: Main
 Name: "{group}\Set Priority";                 Filename: "{app}\448Priority.Exe"; Components: Thread
 Name: "{group}\448Delta Help";                Filename: "{app}\448Delta.Chm";    Components: Help
 Name: "{group}\Uninstall";                    Filename: "{uninstallexe}";        Components: Main
 Name: "{commondesktop}\448Delta Encryption";  Filename: "{app}\448Delta.Exe";   Tasks: DesktopIcon

[Run]
 Filename: "{app}\448Delta.Exe"; Description: "Launch application"; Flags: PostInstall NoWait

[Registry]
;-- Create 448Delta "Single File" Association
 Root: HKCR; Subkey: ".448"; Flags: uninsdeletekey;
 Root: HKCR; Subkey: ".448"; ValueType: string;  ValueData: "448File"
 Root: HKCR; Subkey: "448File"; Flags: uninsdeletekey;
 Root: HKCR; Subkey: "448File"; ValueType: string;  ValueName: "NeverShowExt"; ValueData:""
 Root: HKCR; Subkey: "448File"; ValueType: string;  ValueData:"448Delta Encrypted File"
 Root: HKCR; Subkey: "448File\Shell\Open\Command"; Flags: uninsdeletekey;
 Root: HKCR; Subkey: "448File\Shell\Open\Command"; ValueType: string;  ValueData:"""{app}\448Delta.Exe"" %1"
 Root: HKCR; Subkey: "448File\DefaultIcon"; Flags: uninsdeletekey;
 Root: HKCR; Subkey: "448File\DefaultIcon"; ValueType: string;  ValueData:"{app}\448Delta.Exe,1"

 Root: HKCR; Subkey: "448File\Shell\CreateSelf"; ValueType: string;  ValueData: "Create Self Extractor"; Components: SelfExe
 Root: HKCR; Subkey: "448File\Shell\CreateSelf\Command"; Flags: uninsdeletekey; Components: SelfExe
 Root: HKCR; Subkey: "448File\Shell\CreateSelf\Command"; ValueType: string;  ValueData:"""{app}\SelfAppend.Exe"" %1"; Components: SelfExe

;-- Create 448Delta "Package File" Association
 Root: HKCR; Subkey: ".448Pkg"; Flags: uninsdeletekey;
 Root: HKCR; Subkey: ".448Pkg"; ValueType: string;  ValueData: "448PkgFile"
 Root: HKCR; Subkey: "448PkgFile"; Flags: uninsdeletekey;
 Root: HKCR; Subkey: "448PkgFile"; ValueType: string;  ValueName: "NeverShowExt"; ValueData:""
 Root: HKCR; Subkey: "448PkgFile"; ValueType: string;  ValueData:"448Delta Encrypted Package"
 Root: HKCR; Subkey: "448PkgFile\Shell\Open\Command"; Flags: uninsdeletekey;
 Root: HKCR; Subkey: "448PkgFile\Shell\Open\Command"; ValueType: string;  ValueData:"""{app}\448Delta.Exe"" %1"
 Root: HKCR; Subkey: "448PkgFile\DefaultIcon"; Flags: uninsdeletekey;
 Root: HKCR; Subkey: "448PkgFile\DefaultIcon"; ValueType: string;  ValueData:"{app}\448Delta.Exe,3"

 Root: HKCR; Subkey: "448PkgFile\Shell\CreateSelf"; ValueType: string;  ValueData: "Create Self Extractor"; Components: SelfExe
 Root: HKCR; Subkey: "448PkgFile\Shell\CreateSelf\Command"; Flags: uninsdeletekey; Components: SelfExe
 Root: HKCR; Subkey: "448PkgFile\Shell\CreateSelf\Command"; ValueType: string;  ValueData:"""{app}\SelfAppend.Exe"" %1"; Components: SelfExe

;-----------------------------------------------------------------------------------------------
 Root: HKCR; Subkey: "CLSID\{{121c74b1-d445-4963-9425-edccb905cddb}"; ValueType: string;  ValueData: "448Delta Shell Extension"; Flags: uninsdeletekey;
 Root: HKCR; Subkey: "CLSID\{{121c74b1-d445-4963-9425-edccb905cddb}\InProcServer32"; ValueType: string;  ValueData: "{sys}\448Shell.Dll";
 Root: HKCR; Subkey: "CLSID\{{121c74b1-d445-4963-9425-edccb905cddb}\InProcServer32"; ValueType: string;  ValueName: "ThreadingModel"; ValueData:"Apartment"

;Root: HKCR; Subkey: "*\shellex\ContextMenuHandlers"; ValueType: string;  ValueData:"448Handler"
 Root: HKCR; Subkey: "*\shellex\ContextMenuHandlers\448Handler"; ValueType: string;  ValueData:"{{121c74b1-d445-4963-9425-edccb905cddb}"; Flags: uninsdeletekey;

;Root: HKCR; Subkey: "directory\shellex\ContextMenuHandlers"; ValueType: string;  ValueData:"448Handler"
 Root: HKCR; Subkey: "directory\shellex\ContextMenuHandlers\448Handler"; ValueType: string;  ValueData:"{{121c74b1-d445-4963-9425-edccb905cddb}"; Flags: uninsdeletekey;

 Root: HKLM; Subkey: "SOFTWARE\Microsoft\Windows\CurrentVersion\Shell Extensions\Approved"; ValueType: string;  ValueName: "{{121c74b1-d445-4963-9425-edccb905cddb}"; ValueData:"448Delta Shell Extension"; Flags: deletevalue;
;-----------------------------------------------------------------------------------------------

 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta"; Flags: uninsdeletekey;

 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta";      ValueType: dword;  ValueName: "IsRegistered";  ValueData:0; Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta";      ValueType: binary; ValueName: "RegHash";                    Flags: CreateValueIfDoesntExist;

 ;-- Create Path Keys For Use Within 448Delta
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\Path";
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\Path"; ValueType: string;  ValueName: "Path";          ValueData:"{app}";                Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\Path"; ValueType: string;  ValueName: "SelfExtract";   ValueData:"{app}\SelfExtract.Exe"; Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\Path"; ValueType: string;  ValueName: "SelfAppend";    ValueData:"{app}\SelfAppend.Exe";  Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\Path"; ValueType: string;  ValueName: "448Delta";      ValueData:"{app}\448Delta.Exe";    Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\Path"; ValueType: string;  ValueName: "HelpPath";      ValueData:"{app}\448Delta.Chm";    Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\Path"; ValueType: string;  ValueName: "MenuIcon";      ValueData:"{app}\MenuIcon.Bmp";   Flags: CreateValueIfDoesntExist;

;-- Create Dialog Information Keys For Use Within 448Delta
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\DecryptDialog"; ValueType: dword;  ValueName: "DeleteSourceOption"; ValueData:0; Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\DecryptDialog"; ValueType: dword;  ValueName: "DestroyTempOption";  ValueData:1; Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\DecryptDialog"; ValueType: dword;  ValueName: "SaveCPUOption";      ValueData:0; Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\DecryptDialog"; ValueType: dword;  ValueName: "TopMost";            ValueData:1; Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\DecryptDialog"; ValueType: dword;  ValueName: "UsePriorityOption";  ValueData:1; Flags: CreateValueIfDoesntExist;

 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\EncryptDialog"; ValueType: dword;  ValueName: "CompressOption";     ValueData:0; Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\EncryptDialog"; ValueType: dword;  ValueName: "CreateSelfOption";   ValueData:0; Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\EncryptDialog"; ValueType: dword;  ValueName: "DeleteSourceOption"; ValueData:0; Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\EncryptDialog"; ValueType: dword;  ValueName: "DestroyTempOption";  ValueData:1; Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\EncryptDialog"; ValueType: dword;  ValueName: "SaveCPUOption";      ValueData:0; Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\EncryptDialog"; ValueType: dword;  ValueName: "TopMost";            ValueData:1; Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\EncryptDialog"; ValueType: dword;  ValueName: "UsePriorityOption";  ValueData:1; Flags: CreateValueIfDoesntExist;

 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\StatusDialog";  ValueType: dword;  ValueName: "TopMost";            ValueData:1; Flags: CreateValueIfDoesntExist;

 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\UIDialog";      ValueType: dword;  ValueName: "SaveToDesktop";      ValueData:1; Flags: CreateValueIfDoesntExist;

;-- Create Priority Keys For Use Within 448Delta
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\Priority"; ValueType: dword;  ValueName: "ProcessBoost"; ValueData:1; Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\Priority"; ValueType: dword;  ValueName: "ThreadBoost";  ValueData:1; Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\Priority"; ValueType: dword;  ValueName: "Process";      ValueData:2; Flags: CreateValueIfDoesntExist;
 Root: HKLM; Subkey: "Software\NetWorkDLS\448Delta\Priority"; ValueType: dword;  ValueName: "Thread";       ValueData:4; Flags: CreateValueIfDoesntExist;

