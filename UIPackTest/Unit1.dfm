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
  object RoundedPanel1: TRoundedPanel
    Left = 80
    Top = 128
    Width = 569
    Height = 369
    BorderColor = clGray
    Color = clGreen
    Radius = 20
    RoundedCorner = [LT, RT, LB, RB]
    ShadowEnabled = True
    ShadowColorEnd = clBlack
    ShadowWidthX = 20
    ShadowWidthY = 20
    object RoundedPanel2: TRoundedPanel
      Left = 80
      Top = 56
      Width = 385
      Height = 73
      BorderColor = clGray
      Color = clMaroon
      Radius = 30
      RoundedCorner = [LT, RB]
      ShadowEnabled = True
      ShadowColorEnd = clBlack
      ShadowWidthX = 20
      ShadowWidthY = 20
    end
  end
  object Panel1: TPanel
    Left = 655
    Top = 160
    Width = 385
    Height = 305
    Caption = 'Panel1'
    TabOrder = 1
    object RoundedPanel3: TRoundedPanel
      Left = 80
      Top = 68
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
end
