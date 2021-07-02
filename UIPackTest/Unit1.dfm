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
    Left = 7
    Top = 7
    Width = 357
    Height = 594
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    BorderColor = clGray
    Color = clSkyBlue
    Radius = 10
    RoundedCorner = [LT, RT, LB, RB]
    ShadowWidth = 15
    ShadowColorEnd = clBlack
    object RoundedPanel2: TRoundedPanel
      Left = 63
      Top = 135
      Width = 218
      Height = 362
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      BorderColor = clGray
      Color = clYellow
      Radius = 30
      RoundedCorner = [LT, RT, LB, RB]
      ShadowColorEnd = clBlack
    end
  end
end
