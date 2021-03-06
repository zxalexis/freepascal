{$J+}
unit erroru;
interface

  procedure do_error(l : longint);

  procedure error;

  procedure accept_error(num : longint);

  procedure require_error(num : longint);

function DoMem (Var StartMem : sizeuint): int64;


implementation

const
  program_has_error  : boolean = false;
  accepted_error_num : longint = 0;
  required_error_num : longint = 0;

procedure do_error(l : longint);
begin
  writeln('Error near: ',l);
  halt(100);
end;


procedure error;
begin
   Writeln('Error' {$ifdef FPC_HAS_FEATURE_COMMANDARGS},' in ',paramstr(0){$endif FPC_HAS_FEATURE_COMMANDARGS});
   program_has_error:=true;
end;


procedure accept_error(num : longint);
begin
   accepted_error_num:=num;
end;


procedure require_error(num : longint);
begin
   required_error_num:=num;
   accepted_error_num:=num;
end;


procedure error_unit_exit;
begin
   if exitcode<>0 then
     begin
        if (required_error_num<>0) and (exitcode<>required_error_num) then
          begin
             Write('Program'{$ifdef FPC_HAS_FEATURE_COMMANDARGS},' ',paramstr(0){$endif FPC_HAS_FEATURE_COMMANDARGS});
             Write(' exited with error ',exitcode,' whereas error ');
             Writeln(required_error_num,' was expected');
             Halt(1);
          end
        else if exitcode<>accepted_error_num then
          begin
             Write('Program'{$ifdef FPC_HAS_FEATURE_COMMANDARGS},' ',paramstr(0){$endif FPC_HAS_FEATURE_COMMANDARGS});
             Write(' exited with error ',exitcode,' whereas only error ');
             Writeln(accepted_error_num,' was expected');
             Halt(1);
          end;
     end
   else if required_error_num<>0 then
     begin
        Write('Program'{$ifdef FPC_HAS_FEATURE_COMMANDARGS},' ',paramstr(0){$endif FPC_HAS_FEATURE_COMMANDARGS});
        Write(' exited without error whereas error ');
        Writeln(required_error_num,' was expected');
        Halt(1);
     end;
   if program_has_error then
     Halt(1)
   else
     begin
        exitcode:=0;
{$ifndef CPUJVM}
        erroraddr:=nil;
{$endif ndef CPUJVM}
     end;
end;


function DoMem (Var StartMem : sizeuint): int64;

  function getsize(l:sizeuint):string;
  begin
    if l<16*1024 then
      begin
        str(l,getsize);
        getsize:=getsize+' bytes';
      end
    else
      begin
        str(l shr 10,getsize);
        getsize:=getsize+' Kb';
      end;
  end;

{$ifndef CPUJVM}
var
  hstatus : TFPCHeapstatus;
{$endif ndef CPUJVM}
begin
{$ifndef CPUJVM}
  hstatus:=GetFPCHeapStatus;
  if StartMem=0 then
    begin
      Writeln ('[HEAP] Size: ',getsize(hstatus.CurrHeapSize),',   Used: ',getsize(hstatus.CurrHeapUsed));
      DoMem:=0;
    end
  else
    begin
      Writeln ('[HEAP] Size: ',getsize(hstatus.CurrHeapSize),',   Used: ',getsize(hstatus.CurrHeapUsed),
               ',  Lost: ',getsize(hstatus.CurrHeapUsed-StartMem));
      DoMem:=hstatus.CurrHeapUsed-StartMem;
    end;
  StartMem:=hstatus.CurrHeapUsed;
{$endif ndef CPUJVM}
end;


initialization
finalization
  error_unit_exit;
end.
