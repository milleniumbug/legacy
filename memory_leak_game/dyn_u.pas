unit dyn_u;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, StdCtrls,
  ExtCtrls, DbCtrls;

type

  { TForm1 }

  TForm1 = class(TForm)
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Timer1: TTimer;
    procedure FormCreate(Sender: TObject);
    procedure Klik(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Timer1Timer(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end; 

var
  Form1: TForm1;
  tab : array[0..19, 0..19] of TToggleBox;
  punkty : integer = 0;
  t : integer;

implementation

{$R *.lfm}

{ TForm1 }

procedure TForm1.Klik(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var a,b : integer;
begin
    if TToggleBox(Sender).Checked then
    begin
        inc(punkty,2);
    end else
    begin
        dec(punkty,5);
        TToggleBox(Sender).Checked:=not TToggleBox(Sender).Checked;
    end;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
var i,a,b : integer;
begin
    inc(t);
    Label1.Caption:='Punkty: '+IntToStr(punkty);
    if t = 10 then for i:=1 to 2 do
    begin
        repeat
          a:=random(20);
          b:=random(20);
        until not tab[a,b].Checked;
        tab[a,b].Checked:=true;
        t:=0;
    end;
end;

procedure TForm1.FormCreate(Sender: TObject);
var i,j : integer;
begin
randomize;
for i:=0 to 19 do
    for j:=0 to 19 do
    begin
        tab[i,j]:=TToggleBox.Create(Self);
        tab[i,j].Parent:=Self;
        tab[i,j].Tag:=j*100+i;
        tab[i,j].OnMouseDown:=@Klik;
        tab[i,j].Top:=40+i*20;
        tab[i,j].Left:=40+j*20;
        tab[i,j].Width:=20;
    end;
Label2.Caption:=
'The Plot:'+#13+
#13+
'Jesteś programistą i męczysz się z bugami'+#13+
'w programie - wyciekami pamięci. (może to Firefox?)'+#13+
'Twoim zadaniem jest'+#13+
'naprawienie wszystkich błędów '+#13+
'(buendow? wielbłendow?) w programie'+#13+
'zanim twój debuger wejdzie w nieskończoną pętlę.'+#13+
#13+
'Wersja DEMO'+#13+
'W wersji pełnej wycieki'+#13+
'pamięci są prawdziwe.';
Label3.Caption:=
'Prawa Murphy''iego'+#13+
'1.) Kiedy coś może pójść źle - to pójdzie.'+ #13+
'2.) W każdym programie istnieje jeszcze jeden błąd.';
end;

end.

