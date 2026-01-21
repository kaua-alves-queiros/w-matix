[Setup]
AppName=w-matix
AppVersion=1.0
DefaultDirName={autopf}\w-matix
DefaultGroupName=w-matix
UninstallDisplayIcon={app}\w-matrix.exe
SetupIconFile="xxx"
Compression=lzma
SolidCompression=yes
OutputDir=userdocs:Inno Setup Outputs
OutputBaseFilename=w-matix-installer

[Files]
Source: "xxx"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\w-matix"; Filename: "{app}\w-matrix.exe"
Name: "{autodesktop}\w-matix"; Filename: "{app}\w-matrix.exe"

[Code]
function SendNotifySettingsChange(hWnd: Longint; Msg: Cardinal; wParam: Longint; lParam: String; fuFlags: Cardinal; uTimeout: Cardinal; out lpdwResult: Cardinal): Longint;
  external 'SendMessageTimeoutA@user32.dll stdcall';

procedure UpdatePath();
var
  OldPath: String;
  NewPath: String;
  AppPath: String;
begin
  AppPath := ExpandConstant('{app}');
  if RegQueryStringValue(HKEY_CURRENT_USER, 'Environment', 'Path', OldPath) then
  begin
    if Pos(Uppercase(AppPath), Uppercase(OldPath)) = 0 then
    begin
      NewPath := OldPath + ';' + AppPath;
      RegWriteExpandStringValue(HKEY_CURRENT_USER, 'Environment', 'Path', NewPath);
    end;
  end
  else
  begin
    RegWriteExpandStringValue(HKEY_CURRENT_USER, 'Environment', 'Path', AppPath);
  end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  Res: Cardinal;
begin
  if CurStep = ssPostInstall then
  begin
    UpdatePath();
    SendNotifySettingsChange($FFFF, $001A, 0, 'Environment', 2, 5000, Res);
  end;
end;