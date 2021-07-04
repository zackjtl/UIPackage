object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 622
  ClientWidth = 1474
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -10
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 12
  object Panel1: TPanel
    Left = 655
    Top = 160
    Width = 385
    Height = 305
    Caption = 'Panel1'
    TabOrder = 0
    object RoundedPanel3: TRoundedPanel
      Left = 56
      Top = 52
      Width = 265
      Height = 137
      BorderColor = clGray
      Color = clMaroon
      RoundedCorner = [LT, RT, LB, RB]
      ShadowEnabled = True
      ShadowColorEnd = clBlack
      ShadowWidthX = 20
      ShadowWidthY = 20
    end
  end
  object RoundedPanel2: TRoundedPanel
    Left = 264
    Top = 84
    Width = 265
    Height = 189
    BorderColor = clGray
    Color = clSkyBlue
    Radius = 20
    RoundedCorner = [RT, LB]
    ShadowEnabled = True
    ShadowColorEnd = clBlack
    ShadowWidthX = 9
    ShadowWidthY = 9
  end
end
