unit UFileFunc;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil;

function LoadFileToMemStream(const Filename: string): TMemoryStream;
function n2s(msg:string; n:integer):string;

implementation

function n2s(msg:string; n:integer):string;
var res:string;
begin;
str(n, res); n2s := msg; appendstr(n2s, ' '); appendstr(n2s, res);
end;


function LoadFileToMemStream(const Filename: string): TMemoryStream;
var FileStream: TFileStream;
begin
  Result:=TMemoryStream.Create;
  try
    FileStream:=TFileStream.Create(UTF8ToSys(Filename), fmOpenRead);
    try
      Result.CopyFrom(FileStream,FileStream.Size);
      Result.Position:=0;
    finally
      FileStream.Free;
    end;
  except
    Result.Free;
    Result:=nil;
  end;
end;

end.

