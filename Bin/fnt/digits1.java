package com.game.alex.androidgame.OGLUtils;

import com.game.alex.androidgame.R;

public class EngSimpleFont   extends FontImage {
	static EngSimpleFont    font = new EngSimpleFont()
		{ {
			m_iResID = R.drawable.digits1;
           m_Image = TextureManager.GetInstance().GetTexture(TextureManager.EType.digits1);
           m_sName = "digits1";
           m_iWidth = 120;
           m_iHeight = 1760;
           m_iXoff = 0;
           m_iYoff = 50;
           m_iGlyphHeight = 73;
           m_Glyphs = new GLGlyph[84];
           m_Glyphs[0] = new GLGlyph(' ', 19.000000f, 183.000000f, 81.000000f, 118.000000f, 0.000000f, 0.000000f);
           m_Glyphs[1] = new GLGlyph(' ', 63.000000f, 195.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[2] = new GLGlyph(' ', 75.000000f, 214.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[3] = new GLGlyph(' ', 84.000000f, 229.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[4] = new GLGlyph(' ', 23.000000f, 232.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[5] = new GLGlyph(' ', 13.000000f, 341.000000f, 92.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[6] = new GLGlyph(' ', 74.000000f, 349.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[7] = new GLGlyph(' ', 79.000000f, 353.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[8] = new GLGlyph(' ', 82.000000f, 356.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[9] = new GLGlyph(' ', 88.000000f, 364.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[10] = new GLGlyph(' ', 93.000000f, 383.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[11] = new GLGlyph(' ', 59.000000f, 432.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[12] = new GLGlyph(' ', 105.000000f, 432.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[13] = new GLGlyph(' ', 22.000000f, 499.000000f, 82.000000f, 120.000000f, 0.000000f, 0.000000f);
           m_Glyphs[14] = new GLGlyph(' ', 62.000000f, 531.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[15] = new GLGlyph(' ', 65.000000f, 536.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[16] = new GLGlyph(' ', 70.000000f, 545.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[17] = new GLGlyph(' ', 73.000000f, 550.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[18] = new GLGlyph(' ', 91.000000f, 579.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[19] = new GLGlyph(' ', 103.000000f, 601.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[20] = new GLGlyph(' ', 104.000000f, 607.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[21] = new GLGlyph(' ', 18.000000f, 660.000000f, 80.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[22] = new GLGlyph(' ', 44.000000f, 696.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[23] = new GLGlyph(' ', 45.000000f, 699.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[24] = new GLGlyph(' ', 45.000000f, 701.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[25] = new GLGlyph(' ', 74.000000f, 748.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[26] = new GLGlyph(' ', 78.000000f, 755.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[27] = new GLGlyph(' ', 14.000000f, 820.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[28] = new GLGlyph(' ', 30.000000f, 825.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[29] = new GLGlyph(' ', 81.000000f, 835.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[30] = new GLGlyph(' ', 89.000000f, 847.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[31] = new GLGlyph(' ', 93.000000f, 859.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[32] = new GLGlyph(' ', 33.000000f, 876.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[33] = new GLGlyph(' ', 35.000000f, 876.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[34] = new GLGlyph(' ', 93.000000f, 882.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[35] = new GLGlyph(' ', 107.000000f, 935.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[36] = new GLGlyph(' ', 14.000000f, 977.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[37] = new GLGlyph(' ', 69.000000f, 983.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[38] = new GLGlyph(' ', 71.000000f, 986.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[39] = new GLGlyph(' ', 74.000000f, 993.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[40] = new GLGlyph(' ', 76.000000f, 998.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[41] = new GLGlyph(' ', 16.000000f, 1010.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[42] = new GLGlyph(' ', 103.000000f, 1013.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[43] = new GLGlyph(' ', 103.000000f, 1018.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[44] = new GLGlyph(' ', 56.000000f, 1035.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[45] = new GLGlyph(' ', 101.000000f, 1061.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[46] = new GLGlyph(' ', 105.000000f, 1071.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[47] = new GLGlyph(' ', 16.000000f, 1138.000000f, 92.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[48] = new GLGlyph(' ', 23.000000f, 1149.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[49] = new GLGlyph(' ', 89.000000f, 1158.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[50] = new GLGlyph(' ', 93.000000f, 1167.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[51] = new GLGlyph(' ', 97.000000f, 1183.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[52] = new GLGlyph(' ', 66.000000f, 1195.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[53] = new GLGlyph(' ', 105.000000f, 1235.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[54] = new GLGlyph(' ', 18.000000f, 1302.000000f, 88.000000f, 120.000000f, 0.000000f, 0.000000f);
           m_Glyphs[55] = new GLGlyph(' ', 89.000000f, 1359.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[56] = new GLGlyph(' ', 103.000000f, 1375.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[57] = new GLGlyph(' ', 67.000000f, 1398.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[58] = new GLGlyph(' ', 104.000000f, 1401.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[59] = new GLGlyph(' ', 102.000000f, 1405.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[60] = new GLGlyph(' ', 30.000000f, 1463.000000f, 65.000000f, 120.000000f, 0.000000f, 0.000000f);
           m_Glyphs[61] = new GLGlyph(' ', 59.000000f, 1480.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[62] = new GLGlyph(' ', 68.000000f, 1503.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[63] = new GLGlyph(' ', 73.000000f, 1516.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[64] = new GLGlyph(' ', 74.000000f, 1517.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[65] = new GLGlyph(' ', 76.000000f, 1521.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[66] = new GLGlyph(' ', 76.000000f, 1522.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[67] = new GLGlyph(' ', 89.000000f, 1553.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[68] = new GLGlyph(' ', 89.000000f, 1554.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[69] = new GLGlyph(' ', 91.000000f, 1559.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[70] = new GLGlyph(' ', 95.000000f, 1574.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[71] = new GLGlyph(' ', 13.000000f, 1620.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[72] = new GLGlyph(' ', 75.000000f, 1629.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[73] = new GLGlyph(' ', 19.000000f, 1632.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[74] = new GLGlyph(' ', 16.000000f, 1637.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[75] = new GLGlyph(' ', 42.000000f, 1648.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[76] = new GLGlyph(' ', 40.000000f, 1655.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[77] = new GLGlyph(' ', 40.000000f, 1659.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[78] = new GLGlyph(' ', 40.000000f, 1663.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[79] = new GLGlyph(' ', 99.000000f, 1663.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[80] = new GLGlyph(' ', 45.000000f, 1683.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[81] = new GLGlyph(' ', 107.000000f, 1701.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[82] = new GLGlyph(' ', 107.000000f, 1707.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
           m_Glyphs[83] = new GLGlyph(' ', 105.000000f, 1719.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f);
        } };    static public  EngSimpleFont GetInstance() { return font; };}