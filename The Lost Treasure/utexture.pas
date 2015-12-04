unit UTexture;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, UFileFunc, GL, GLU;

type CTexture = object
public
Width,Height: longint;
uiStor:LongWord;
Data : pointer;
fAddU, fAddV, fCurU, fCurV, fCurAn : single;
bLiquid : BYTE;

procedure loadBMP(filename:string; iFilter:integer);
procedure loadBMPColorKey(filename:string; r, g, b : BYTE);
procedure bindTexture;
end;

var tTextures: array[1..50] of CTexture;
var tSkyBox: array[0..5] of CTexture;

implementation

//////// CTEXTURE /////////*
//
// OpenGL Texture class.
//
//////// CTEXTURE /////////*


function LoadglTexImage2DFromBitmapFileColorKey(Filename:string; var Image:CTexture; r, g, b : byte): boolean;
type
  TBITMAPFILEHEADER = packed record
    bfType: Word;
    bfSize: DWORD;
    bfReserved1: Word;
    bfReserved2: Word;
    bfOffBits: DWORD;
  end;

  BITMAPINFOHEADER = packed record
          biSize : DWORD;
          biWidth : Longint;
          biHeight : Longint;
          biPlanes : WORD;
          biBitCount : WORD;
          biCompression : DWORD;
          biSizeImage : DWORD;
          biXPelsPerMeter : Longint;
          biYPelsPerMeter : Longint;
          biClrUsed : DWORD;
          biClrImportant : DWORD;
       end;

  RGBQUAD = packed record
          rgbBlue : BYTE;
          rgbGreen : BYTE;
          rgbRed : BYTE;
       //   rgbReserved : BYTE;
       end;

  BITMAPINFO = packed record
          bmiHeader : BITMAPINFOHEADER;
          bmiColors : array[0..0] of RGBQUAD;
       end;

  PBITMAPINFO = ^BITMAPINFO;

  TRawImage = packed record
     p:array[0..0] of byte;
   end;
  PRawImage = ^TRawImage;

const
  BI_RGB = 0;

var
  MemStream: TMemoryStream;
  BmpHead: TBitmapFileHeader;
  BmpInfo:PBitmapInfo;
  ImgSize:longint;
  InfoSize, PixelCount, i:integer;
  BitsPerPixel:integer;
  AnRGBQuad: RGBQUAD;
begin
  Result:=false;
  MemStream:=LoadFileToMemStream(Filename);
  if MemStream=nil then exit;
  try
    if (MemStream.Read(BmpHead, sizeof(BmpHead))<sizeof(BmpHead))
    or (BmpHead.bfType <> $4D42) then begin
      writeln('Invalid windows bitmap (header)');
      exit;
    end;
    InfoSize:=BmpHead.bfOffBits-SizeOf(BmpHead);
    GetMem(BmpInfo,InfoSize);
    try
      if MemStream.Read(BmpInfo^,InfoSize)<>InfoSize then begin
        writeln('Invalid windows bitmap (info)');
        exit;
      end;
      if BmpInfo^.bmiHeader.biSize<>sizeof(BitmapInfoHeader) then begin
        writeln('OS2 bitmaps are not supported yet');
        exit;
      end;
      if BmpInfo^.bmiHeader.biCompression<>bi_RGB then begin
        writeln('RLE compression is not supported yet');
        exit;
      end;
      BitsPerPixel:=BmpInfo^.bmiHeader.biBitCount;
      if BitsPerPixel<>24 then begin
        writeln('Only truecolor bitmaps supported yet');
        exit;
      end;
      ImgSize:=BmpInfo^.bmiHeader.biSizeImage;
      if MemStream.Size-MemStream.Position<ImgSize then begin
        writeln('Invalid windows bitmap (bits)');
        exit;
      end;
      Image.Width:=BmpInfo^.bmiHeader.biWidth;
      Image.Height:=BmpInfo^.bmiHeader.biHeight;
      PixelCount:=Image.Width*Image.Height;
      GetMem(Image.Data,PixelCount * 4);
      try
        for i:=0 to PixelCount-1 do begin
          MemStream.Read(AnRGBQuad,sizeOf(RGBQuad));
          with PRawImage(Image.Data)^ do begin
            p[i*4+0]:=AnRGBQuad.rgbRed;
            p[i*4+1]:=AnRGBQuad.rgbGreen;
            p[i*4+2]:=AnRGBQuad.rgbBlue;
            if(AnRGBQuad.rgbRed = r) and (AnRGBQuad.rgbGreen = g) and (AnRGBQuad.rgbBlue = b) then p[i*4+3] := 0 else p[i*4+3] := 255;
          end;
        end;
      except
        writeln('Error converting bitmap');
        FreeMem(Image.Data);
        Image.Data:=nil;
        exit;
      end;
    finally
      FreeMem(BmpInfo);
    end;
    Result:=true;
  finally
    MemStream.Free;
  end;
  Result:=true;
end;

function LoadglTexImage2DFromBitmapFile(Filename:string; var Image:CTexture): boolean;
type
  TBITMAPFILEHEADER = packed record
    bfType: Word;
    bfSize: DWORD;
    bfReserved1: Word;
    bfReserved2: Word;
    bfOffBits: DWORD;
  end;

  BITMAPINFOHEADER = packed record
          biSize : DWORD;
          biWidth : Longint;
          biHeight : Longint;
          biPlanes : WORD;
          biBitCount : WORD;
          biCompression : DWORD;
          biSizeImage : DWORD;
          biXPelsPerMeter : Longint;
          biYPelsPerMeter : Longint;
          biClrUsed : DWORD;
          biClrImportant : DWORD;
       end;

  RGBQUAD = packed record
          rgbBlue : BYTE;
          rgbGreen : BYTE;
          rgbRed : BYTE;
       //   rgbReserved : BYTE;
       end;

  BITMAPINFO = packed record
          bmiHeader : BITMAPINFOHEADER;
          bmiColors : array[0..0] of RGBQUAD;
       end;

  PBITMAPINFO = ^BITMAPINFO;

  TRawImage = packed record
     p:array[0..0] of byte;
   end;
  PRawImage = ^TRawImage;

const
  BI_RGB = 0;

var
  MemStream: TMemoryStream;
  BmpHead: TBitmapFileHeader;
  BmpInfo:PBitmapInfo;
  ImgSize:longint;
  InfoSize, PixelCount, i:integer;
  BitsPerPixel:integer;
  AnRGBQuad: RGBQUAD;
begin
  Result:=false;
  MemStream:=LoadFileToMemStream(Filename);
  if MemStream=nil then exit;
  try
    if (MemStream.Read(BmpHead, sizeof(BmpHead))<sizeof(BmpHead))
    or (BmpHead.bfType <> $4D42) then begin
      writeln('Invalid windows bitmap (header)');
      exit;
    end;
    InfoSize:=BmpHead.bfOffBits-SizeOf(BmpHead);
    GetMem(BmpInfo,InfoSize);
    try
      if MemStream.Read(BmpInfo^,InfoSize)<>InfoSize then begin
        writeln('Invalid windows bitmap (info)');
        exit;
      end;
      if BmpInfo^.bmiHeader.biSize<>sizeof(BitmapInfoHeader) then begin
        writeln('OS2 bitmaps are not supported yet');
        exit;
      end;
      if BmpInfo^.bmiHeader.biCompression<>bi_RGB then begin
        writeln('RLE compression is not supported yet');
        exit;
      end;
      BitsPerPixel:=BmpInfo^.bmiHeader.biBitCount;
      if BitsPerPixel<>24 then begin
        writeln('Only truecolor bitmaps supported yet');
        exit;
      end;
      ImgSize:=BmpInfo^.bmiHeader.biSizeImage;
      if MemStream.Size-MemStream.Position<ImgSize then begin
        writeln('Invalid windows bitmap (bits)');
        exit;
      end;
      Image.Width:=BmpInfo^.bmiHeader.biWidth;
      Image.Height:=BmpInfo^.bmiHeader.biHeight;
      PixelCount:=Image.Width*Image.Height;
      GetMem(Image.Data,PixelCount * 3);
      try
        for i:=0 to PixelCount-1 do begin
          MemStream.Read(AnRGBQuad,sizeOf(RGBQuad));
          with PRawImage(Image.Data)^ do begin
            p[i*3+0]:=AnRGBQuad.rgbRed;
            p[i*3+1]:=AnRGBQuad.rgbGreen;
            p[i*3+2]:=AnRGBQuad.rgbBlue;
          end;
        end;
      except
        writeln('Error converting bitmap');
        FreeMem(Image.Data);
        Image.Data:=nil;
        exit;
      end;
    finally
      FreeMem(BmpInfo);
    end;
    Result:=true;
  finally
    MemStream.Free;
  end;
  Result:=true;
end;

procedure CTexture.loadBMP(filename:string; iFilter:integer);
begin;
	LoadGLTexImage2DFromBitmapFile(filename, self);
	glGenTextures(1, @uiStor);
	glBindTexture(GL_TEXTURE_2D, uiStor);
	// Set its parameters
   if(iFilter = 0)then
   begin;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
   end else if(iFilter = 1)then
   begin;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
   end else if(iFilter = 2)then
   begin;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, Data);
	end;
end;

procedure CTexture.loadBMPColorKey(filename:string; r, g, b : BYTE);
begin;
	LoadGLTexImage2DFromBitmapFileColorKey(filename, self, r, g, b);
	glGenTextures(1, @uiStor);
	glBindTexture(GL_TEXTURE_2D, uiStor);
	// Set its parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);

end;

procedure CTexture.bindTexture;
begin;
	glBindTexture(GL_TEXTURE_2D, uiStor);
end;


end.

