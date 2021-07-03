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
    Left = 0
    Top = 0
    Width = 1474
    Height = 622
    Align = alClient
    Caption = 'Panel1'
    DoubleBuffered = False
    ParentDoubleBuffered = False
    TabOrder = 0
    object RoundedPanel1: TRoundedPanel
      Left = 132
      Top = 160
      Width = 261
      Height = 369
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      BorderColor = clGray
      Color = clSkyBlue
      Radius = 15
      RoundedCorner = [LT, RT, LB, RB]
      ShadowColorEnd = clBlack
    end
    object RoundedPanel2: TRoundedPanel
      Left = 655
      Top = 103
      Width = 218
      Height = 362
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      BorderColor = clGray
      Color = clYellow
      BorderType = btNormal
      RoundedCorner = [LT, RT, LB, RB]
      ShadowWidth = 10
      ShadowColorEnd = clBlack
      object RoundedPanel3: TRoundedPanel
        Left = 16
        Top = 112
        Width = 169
        Height = 97
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        BorderColor = clGray
        Color = clTeal
        BorderType = btNormal
        RoundedCorner = [RT, LB]
        ShadowWidth = 10
        ShadowColorEnd = clBlack
      end
    end
  end
end
