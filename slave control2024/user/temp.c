code linh tinh
#ifdef _U1_DEBUG_ENABLE_
		printf("new timestamp:%d, %d\r\n",timenow, timeOutLostSignal);
		timeinfo = localtime( &timenow );
		printf("Current local time and date: %s\r\n", asctime(timeinfo));
		#endif
		
		
unsigned char  _sysfont[] =
{

  'F', 2, 0, 0, 0, 255, 8,
  0,		// 										0 - 'Empty Cell'; 7
  5, 62, 91, 79, 91, 62,		// 1 - 'Sad Smiley';8
  5, 62, 107, 79, 107, 62,		// 2 - 'Happy Smiley';14
  5, 28, 62, 124, 62, 28,		// 3 - 'Heart';20
  5, 24, 60, 126, 60, 24,		// 4 - 'Diamond';26
  5, 28, 87, 125, 87, 28,		// 5 - 'Clubs';32
  5, 28, 94, 127, 94, 28,		// 6 - 'Spades';38
  4, 0, 24, 60, 24,		// 7 - 'Bullet Point';44
  5, 255, 231, 195, 231, 255,		// 8 - 'Rev Bullet Point';49
  4, 0, 24, 36, 24,		// 9 - 'Hollow Bullet Point';55
  5, 255, 231, 219, 231, 255,		// 10 - 'Rev Hollow BP';60
  5, 48, 72, 58, 6, 14,		// 11 - 'Male';66
  5, 38, 41, 121, 41, 38,		// 12 - 'Female';72
  5, 64, 127, 5, 5, 7,		// 13 - 'Music Note 1';78
  5, 64, 127, 5, 37, 63,		// 14 - 'Music Note 2';84
  5, 90, 60, 231, 60, 90,		// 15 - 'Snowflake';90
  5, 127, 62, 28, 28, 8,		// 16 - 'Right Pointer';96
  5, 8, 28, 28, 62, 127,		// 17 - 'Left Pointer';102
  5, 20, 34, 127, 34, 20,		// 18 - 'UpDown Arrows';108
  5, 255, 255, 255, 255, 255,		// 19 - 'Full Block';114
  5, 240, 240, 240, 240, 240,		// 20 - 'Half Block Bottom';120
  3, 255, 255, 255,		// 21 - 'Half Block LHS';126
  5, 0, 0, 0, 255, 255,		// 22 - 'Half Block RHS';130
  5, 15, 15, 15, 15, 15,		// 23 - 'Half Block Top';136
  5, 8, 4, 126, 4, 8,		// 24 - 'Up Arrow';142
  5, 16, 32, 126, 32, 16,		// 25 - 'Down Arrow';148
  5, 8, 8, 42, 28, 8,		// 26 - 'Right Arrow';154
  5, 8, 28, 42, 8, 8,		// 27 - 'Left Arrow';160
  5, 170, 0, 85, 0, 170,		// 28 - '30% shading';166
  5, 170, 85, 170, 85, 170,		// 29 - '50% shading';172
  5, 48, 56, 62, 56, 48,		// 30 - 'Up Pointer';178
  5, 6, 14, 62, 14, 6,		// 31 - 'Down Pointer';184
  2, 0, 0, 		// 32 - 'Space';190
  1, 95,		// 33 - '!';193
  3, 7, 0, 7,		// 34 - '""'/195
  5, 20, 127, 20, 127, 20,		// 35 - '#';199
  5, 68, 74, 255, 74, 50,		// 36 - '$';205
  5, 99, 19, 8, 100, 99,		// 37 - '%';211
  5, 54, 73, 73, 54, 72,		// 38 - '&';217
  1, 7,		// 39 - ''';223
  3, 62, 65, 65,		// 40 - '(';225
  3, 65, 65, 62,		// 41 - ')';229
  5, 8, 42, 28, 42, 8,		// 42 - '*';233
  5, 8, 8, 62, 8, 8,		// 43 - '+';239
  2, 96, 224,		// 44 - ',';245
  4, 8, 8, 8, 8,		// 45 - '-';248
  2, 96, 96,		// 46 - '.';253
  5, 96, 16, 8, 4, 3,		// 47 - '/';256
  5, 62, 81, 73, 69, 62,		// 					48 - '0'       262
  3, 0,4, 2, 127,0,		// 										49 - '1'                 268
//  3, 66, 127, 64,		// 49 - serifed '1'
  5, 113, 73, 73, 73, 70,							// 50 - '2'       272
  5, 65, 73, 73, 73, 54,							// 51 - '3'       278
  5, 15, 8, 8, 8, 127,								// 52 - '4'         284
  5, 79, 73, 73, 73, 49,							// 53 - '5'       290
  5, 62, 73, 73, 73, 48,							// 54 - '6'       296
  5, 3, 1, 1, 1, 127,									// 55 - '7'           302
//  5, 1, 1, 121, 5, 3,		// 55 - european style '7'
  5, 54, 73, 73, 73, 54,							// 56 - '8'       311
  5, 6, 73, 73, 73, 62,								// 57 - '9'         317 
  2, 108, 108,												// 58 - ':'                 318
  2, 108, 236,		// 59 - ';'
  3, 8, 20, 34,		// 60 - '<'
  4, 20, 20, 20, 20,		// 61 - '='
  3, 34, 20, 8,		// 62 - '>'
  5, 1, 89, 9, 9, 6,		// 63 - '?'
  5, 62, 65, 93, 89, 78,		// 64 - '@'
  5, 126, 9, 9, 9, 126,		// 65 - 'A'
  5, 127, 73, 73, 73, 54,		// 66 - 'B'
  5, 62, 65, 65, 65, 65,		// 67 - 'C'
  5, 127, 65, 65, 65, 62,		// 68 - 'D'
  5, 127, 73, 73, 73, 65,		// 69 - 'E'
  5, 127, 9, 9, 9, 1,		// 70 - 'F'
  5, 62, 65, 65, 73, 121,		// 71 - 'G'
  5, 127, 8, 8, 8, 127,		// 72 - 'H'
  3, 65, 127, 65,		// 73 - 'I'
  5, 48, 65, 65, 65, 63,		// 74 - 'J'
  5, 127, 8, 20, 34, 65,		// 75 - 'K'
  5, 127, 64, 64, 64, 64,		// 76 - 'L'
  5, 127, 2, 12, 2, 127,		// 77 - 'M'
  5, 127, 4, 8, 16, 127,		// 78 - 'N'
  5, 62, 65, 65, 65, 62,		// 79 - 'O'
  5, 127, 9, 9, 9, 6,		// 80 - 'P'
  5, 62, 65, 65, 97, 126,		// 81 - 'Q'
  5, 127, 9, 25, 41, 70,		// 82 - 'R'
  5, 70, 73, 73, 73, 49,		// 83 - 'S'
  5, 1, 1, 127, 1, 1,		// 84 - 'T'
  5, 63, 64, 64, 64, 63,		// 85 - 'U'
  5, 31, 32, 64, 32, 31,		// 86 - 'V'
  5, 63, 64, 56, 64, 63,		// 87 - 'W'
  5, 99, 20, 8, 20, 99,		// 88 - 'X'
  5, 3, 4, 120, 4, 3,		// 89 - 'Y'
  5, 97, 81, 73, 69, 67,		// 90 - 'Z'
  3, 127, 65, 65,		// 91 - '['
  5, 3, 4, 8, 16, 96,		// 92 - '\'
  3, 65, 65, 127,		// 93 - ']'
  5, 4, 2, 1, 2, 4,		// 94 - '^'
  4, 128, 128, 128, 128,		// 95 - '_'
  3, 1, 2, 4,		// 96 - '`'
  4, 56, 68, 68, 124,		// 97 - 'a'
  4, 127, 68, 68, 56,		// 98 - 'b'
  4, 56, 68, 68, 68,		// 99 - 'c'
  4, 56, 68, 68, 127,		// 100 - 'd'
  4, 56, 84, 84, 88,		// 101 - 'e'
  4, 4, 126, 5, 1,		// 102 - 'f'
  4, 24, 164, 164, 124,		// 103 - 'g'
  4, 127, 4, 4, 120,		// 104 - 'h'
  1, 125,		// 105 - 'i'
  3, 132, 133, 124,		// 106 - 'j'
  4, 127, 16, 40, 68,		// 107 - 'k'
  1, 127,		// 108 - 'l'
  5, 124, 4, 120, 4, 120,		// 109 - 'm'
  4, 124, 4, 4, 120,		// 110 - 'n'
  4, 56, 68, 68, 56,		// 111 - 'o'
  4, 252, 36, 36, 24,		// 112 - 'p'
  4, 24, 36, 36, 252,		// 113 - 'q'
  4, 124, 4, 4, 8,		// 114 - 'r'
  4, 88, 84, 84, 52,		// 115 - 's'
  3, 4, 127, 4,		// 116 - 't'
  4, 60, 64, 64, 124,		// 117 - 'u'
  4, 28, 32, 64, 124,		// 118 - 'v'
  5, 60, 64, 48, 64, 60,		// 119 - 'w'
  4, 108, 16, 16, 108,		// 120 - 'x'
  4, 28, 160, 160, 124,		// 121 - 'y'
  4, 100, 84, 84, 76,		// 122 - 'z'
  4, 8, 54, 65, 65,		// 123 - '{'
  1, 127,		// 124 - '|'
  4, 65, 65, 54, 8,		// 125 - '}'
  4, 2, 1, 2, 1,		// 126 - '~'
  5, 127, 65, 65, 65, 127,		// 127 - 'Hollow Block'
  5, 62, 85, 85, 85, 65,		// 128 - 'Euro symbol'
  5, 56, 68, 68, 56, 68,		// 129 - 'Alpha'
  5, 124, 42, 42, 62, 20,		// 130 - 'Beta'
  5, 126, 2, 2, 6, 6,		// 131 - 'Gamma'
  5, 2, 126, 2, 126, 2,		// 132 - 'Pi'
  5, 99, 85, 73, 65, 99,		// 133 - 'Sigma'
  5, 56, 68, 68, 60, 4,		// 134 - 'Theta'
  5, 64, 126, 32, 30, 32,		// 135 - 'mu'
  5, 6, 2, 126, 2, 2,		// 136 - 'Tau'
  8, 99, 19, 8, 100, 99, 0, 96, 96,		// 137 - 'Percent 00'
  5, 42, 42, 42, 42, 42,		// 138 - '3 Bar Equals'
  3, 81, 74, 68,		// 139 - '>='
  3, 68, 74, 81,		// 140 - '<='
  5, 0, 0, 255, 1, 3,		// 141 - 'Top of Integral'
  3, 224, 128, 255,		// 142 - 'Bot of Integral'
  5, 54, 18, 54, 36, 54,		// 143 - 'Wavy ='
  3, 2, 5, 2,		// 144 - 'Degree'
  2, 24, 24,		// 145 - 'Math Product'
  2, 16, 16,		// 146 - 'Short Dash'
  5, 48, 64, 255, 1, 1,		// 147 - 'Square Root'
  4, 31, 1, 1, 30,		// 148 - 'Superscript n'
  0,		// 149
  0,		// 150  
  0,		// 151  
  0,		// 152
  9, 1, 1, 127, 1, 127, 2, 12, 2, 127,		// 153 - 'Trademark'
  0,		// 154
  0,		// 155
  0,		// 156
  0,		// 157
  0,		// 158
  0,		// 159
  1, 0,		// 160 - ' Non-breaking space'
  1, 125,		// 161 - '¡ Inverted Exclamation Mark'
  4, 24, 36, 126, 36,		// 162 - '¢ Cent sign'
  4, 68, 126, 69, 65,		// 163 - '£ Pound sign'
  5, 34, 28, 20, 28, 34,		// 164 - '¤ Currency sign'
  5, 1, 42, 124, 42, 1,		// 165 - '¥ Yen sign'
  1, 119,		// 166 - '¦ Broken bar'
  4, 78, 85, 85, 57,		// 167 - '§ Section sign'
  3, 2, 0, 2,		// 168 - '¨ Diaeresis (Umlaut)'
  7, 126, 129, 189, 165, 165, 129, 126,		// 169 - '© Copyright sign'
  3, 38, 41, 47,		// 170 - 'ª Feminine Ordinal Indicator'
  5, 8, 20, 42, 20, 34,		// 171 - '« Left-pointing double angle quotation mark'
  4, 8, 8, 8, 24,		// 172 - '¬ Not sign'
  3, 8, 8, 8,		// 173 - ' Soft hyphen'
  7, 126, 129, 189, 149, 169, 129, 126,		// 174 - '® Registered sign'
  5, 1, 1, 1, 1, 1,		// 175 - '¯ macron'
  3, 2, 5, 2,		// 176 - '° Degree symbol'
  3, 36, 46, 36,		// 177 - '± Plus-minus sign'
  3, 25, 21, 18,		// 178 - '² Superscript two'
  3, 21, 21, 10,		// 179 - '³ Superscript three'
  2, 2, 1,		// 180 - '´ Acute accent'
  4, 248, 32, 64, 120,		// 181 - 'µ Micro sign'
  5, 6, 9, 127, 1, 127,		// 182 - 'Pilcrow sign'
  2, 24, 24,		// 183 - '· Middle dot'
  4, 0, 128, 160, 192,		// 184 - '¸ Cedilla'
  3, 18, 31, 16,		// 185 - '¹ Superscript one'
  3, 38, 41, 38,		// 186 - 'º Masculine ordinal indicator'
  5, 34, 20, 42, 20, 8,		// 187 - '» Right-pointing double angle quotation mark'
  8, 64, 47, 16, 8, 4, 30, 17, 124,		// 188 - '¼ Vulgar fraction one quarter'
  8, 64, 47, 16, 8, 4, 98, 85, 76,		// 189 - '½ Vulgar fraction one half'
  9, 21, 85, 63, 16, 8, 4, 30, 17, 124,		// 190 - '¾ Vulgar fraction three quarters'
  5, 48, 72, 72, 77, 64,		// 191 - '¿ Inverted Question Mark'
  5, 120, 21, 22, 20, 120,		// 192 - 'À Latin Capital Letter A with grave'
  5, 120, 20, 22, 21, 120,		// 193 - 'Á Latin Capital letter A with acute'
  5, 122, 21, 20, 21, 122,		// 194 - 'Â Latin Capital letter A with circumflex'
  5, 120, 22, 21, 22, 121,		// 195 - 'Ã Latin Capital letter A with tilde'
  5, 120, 21, 20, 21, 120,		// 196 - 'Ä Latin Capital letter A with diaeresis'
  5, 120, 20, 21, 20, 120,		// 197 - 'Å Latin Capital letter A with ring above'
  7, 126, 9, 9, 127, 73, 73, 65,		// 198 - 'Æ Latin Capital letter Æ'
  5, 158, 161, 97, 33, 33,		// 199 - 'Ç Latin Capital letter C with cedilla'
  5, 124, 84, 85, 70, 68,		// 200 - 'È Latin Capital letter E with grave'
  5, 124, 84, 86, 69, 68,		// 201 - 'É Latin Capital letter E with acute'
  5, 126, 85, 84, 69, 70,		// 202 - 'Ê Latin Capital letter E with circumflex'
  5, 124, 85, 84, 69, 68,		// 203 - 'Ë Latin Capital letter E with diaeresis'
  3, 69, 126, 68,		// 204 - 'Ì Latin Capital letter I with grave'
  3, 68, 126, 69,		// 205 - 'Í Latin Capital letter I with acute'
  3, 70, 125, 70,		// 206 - 'Î Latin Capital letter I with circumflex'
  3, 69, 124, 69,		// 207 - 'Ï Latin Capital letter I with diaeresis'
  5, 8, 127, 73, 65, 62,		// 208 - 'Ð Latin Capital letter Eth'
  5, 124, 10, 17, 34, 125,		// 209 - 'Ñ Latin Capital letter N with tilde'
  5, 56, 68, 69, 70, 56,		// 210 - 'Ò Latin Capital letter O with grave'
  5, 56, 68, 70, 69, 56,		// 211 - 'Ó Latin Capital letter O with acute'
  5, 58, 69, 68, 69, 58,		// 212 - 'Ô Latin Capital letter O with circumflex'
  5, 56, 70, 69, 70, 57,		// 213 - 'Õ Latin Capital letter O with tilde'
  5, 56, 69, 68, 69, 56,		// 214 - 'Ö Latin Capital letter O with diaeresis'
  5, 34, 20, 8, 20, 34,		// 215 - '× Multiplication sign'
  5, 124, 98, 90, 70, 62,		// 216 - 'Ø Latin Capital letter O with stroke'
  5, 60, 64, 65, 66, 60,		// 217 - 'Ù Latin Capital letter U with grave'
  5, 60, 64, 66, 65, 60,		// 218 - 'Ú Latin Capital letter U with acute'
  5, 60, 66, 65, 66, 60,		// 219 - 'Û Latin Capital Letter U with circumflex'
  5, 60, 65, 64, 65, 60,		// 220 - 'Ü Latin Capital Letter U with diaeresis'
  5, 2, 4, 122, 5, 2,		// 221 - 'Ý Latin Capital Letter Y with acute'
  4, 63, 18, 18, 12,		// 222 - 'Þ Latin Capital Letter Thorn'
  5, 126, 73, 73, 78, 48,		// 223 - 'ß Latin Small Letter sharp S'
  4, 56, 69, 70, 124,		// 224 - 'à Latin Small Letter A with grave'
  4, 56, 68, 70, 125,		// 225 - 'á Latin Small Letter A with acute'
  4, 56, 70, 69, 126,		// 226 - 'â Latin Small Letter A with circumflex'
  4, 58, 69, 70, 125,		// 227 - 'ã Latin Small Letter A with tilde'
  4, 56, 69, 68, 125,		// 228 - 'ä Latin Small Letter A with diaeresis'
  4, 48, 74, 77, 122,		// 229 - 'å Latin Small Letter A with ring above'
  7, 32, 84, 84, 56, 84, 84, 88,		// 230 - 'æ Latin Small Letter Æ'
  4, 156, 162, 98, 34,		// 231 - 'ç Latin Small Letter C with cedilla'
  4, 56, 85, 86, 88,		// 232 - 'è Latin Small Letter E with grave'
  4, 56, 84, 86, 89,		// 233 - 'é Latin Small Letter E with acute'
  4, 56, 86, 85, 90,		// 234 - 'ê Latin Small Letter E with circumflex'
  4, 56, 85, 84, 89,		// 235 - 'ë Latin Small Letter E with diaeresis'
  2, 1, 122,		// 236 - 'ì Latin Small Letter I with grave'
  2, 122, 1,		// 237 - 'í Latin Small Letter I with acute'
  3, 2, 121, 2,		// 238 - 'î Latin Small Letter I with circumflex'
  3, 2, 120, 2,		// 239 - 'ï Latin Small Letter I with diaeresis'
  4, 48, 75, 75, 60,		// 240 - 'ð Latin Small Letter Eth'
  4, 122, 9, 10, 113,		// 241 - 'ñ Latin Small Letter N with tilde'
  4, 48, 73, 74, 48,		// 242 - 'ò Latin Small Letter O with grave'
  4, 48, 72, 74, 49,		// 243 - 'ó Latin Small Letter O with acute'
  4, 48, 74, 73, 50,		// 244 - 'ô Latin Small Letter O with circumflex'
  4, 50, 73, 74, 49,		// 245 - 'õ Latin Small Letter O with tilde'
  4, 57, 68, 68, 57,		// 246 - 'ö Latin Small Letter O with diaeresis'
  5, 8, 8, 42, 8, 8,		// 247 - '÷ Division sign'
  4, 56, 84, 76, 56,		// 248 - 'ø Latin Small Letter O with stroke'
  4, 56, 65, 66, 120,		// 249 - 'ù Latin Small Letter U with grave'
  4, 56, 64, 66, 121,		// 250 - 'ú Latin Small Letter U with acute'
  4, 56, 66, 65, 122,		// 251 - 'û Latin Small Letter U with circumflex'
  4, 58, 64, 64, 122,		// 252 - 'ü Latin Small Letter U with diaeresis'
  4, 24, 160, 162, 121,		// 253 - 'ý Latin Small Letter Y with acute'
  4, 252, 40, 40, 16,		// 254 - 'þ Latin Small Letter Thorn'
  4, 26, 160, 160, 122,		// 255 - 'ÿ Latin Small Letter Y with diaeresis'

};
//void BCD_Decoder()
//{
//	//printf("i2c_rv[3]: %d",i2c_rv[3]);
//	for(char x=0;x<7;x++) ds3231_reg[x]=(i2c_rv[x] & 0x0f) + (i2c_rv[x]>>4)*10;
//	//ds3231_reg[3] --; 
//}

		//RTC second
		//RTC hoat dong binh thuong
		//rtc_timeout = 0;
		
  	
		//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
		//printf("SQW\r\n");
		//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);

//	

//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
//		printf("Factory reset\r\n");
//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);

		//Receied NTP message, processing and respond
		//wzn_event_handle();
		//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
		//printf("NTP ngat\r\n");
		//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
//void currentUptime(void *ptr, uint8_t *len)
//	{}
//void setMyValue(int value)	//snmpset -v 1 -c public 192.168.1.246 .1.3.6.1.4.1.6.1.2 i 123456
//{
//	printf("Got my value :%d\r\n",value);
//}
/*
void getMyValue()						//snmpget -v 1 -c public 192.168.1.246 .1.3.6.1.4.1.6.1.2
{
	printf("getMyValue\r\n");
	//return 301188;
}*/

/*
    // Example Codes for SNMP Trap
    {
		dataEntryType enterprise_oid = {0x0a, {0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0x9b, 0x19, 0x01, 0x00},
    	    									SNMPDTYPE_OBJ_ID, 0x0a, {"\x2b\x06\x01\x04\x01\x81\x9b\x19\x10\x00"},	NULL, NULL};

		dataEntryType trap_oid1 = {8, {0x2b, 6, 1, 4, 1, 0, 11, 0}, SNMPDTYPE_OCTET_STRING, 30, {""}, NULL, NULL};
		dataEntryType trap_oid2 = {8, {0x2b, 6, 1, 4, 1, 0, 12, 0}, SNMPDTYPE_INTEGER, 4, {""}, NULL, NULL};

		strcpy((char *)trap_oid1.u.octetstring, "Alert!!!"); 	// String added
		trap_oid2.u.intval = 123456;	// Integer value added

		// Generic Trap: warmStart
		snmp_sendTrap((void *)"192.168.0.214", (void *)"192.168.0.112", (void *)"public", enterprise_oid, SNMPTRAP_WARMSTART, 0, 0);

		// Enterprise-Specific Trap
		snmp_sendTrap((void *)"192.168.0.214", (void *)"192.168.0.112", (void *)"public", enterprise_oid, 6, 0, 2, &trap_oid1, &trap_oid2);
	}*/
	
	#ifdef _SNMP_DEBUG_
void dumpCode(uint8_t* header, uint8_t* tail, uint8_t *buff, int32_t len)
{
	int i;

	printf((char const*)header);

	for (i=0; i<len; i++)
	{
		if ( i%16==0 )	printf("0x%04x : ", i);
		printf("%02x ",buff[i]);

		if ( i%16-15==0 )
		{
			int j;
			printf("  ");
			for (j=i-15; j<=i; j++)
			{
				if ( isprint(buff[j]) )	printf("%c", buff[j]);
				else					printf(".");
			}
			printf("\r\n");
		}
	}

	if ( i%16!=0 )
	{
		int j;
		int spaces=(len-i+16-i%16)*3+2;
		for (j=0; j<spaces; j++) 	printf(" ");
		for (j=i-i%16; j<len; j++)
		{
			if ( isprint(buff[j]) )	printf("%c", buff[j]);
			else					printf(".");
		}
	}
	printf((char const*)tail);
}
#endif

/**
int32_t snmpd_run(void)
{
  int32_t ret;
	int32_t len = 0;
  //uint8_t a[4] = {192,168,22,164};  
	uint8_t svr_addr[6];
	uint16_t  svr_port;

	if(SOCK_SNMP_AGENT > _WIZCHIP_SOCK_NUM_) return -99;
    
	switch(getSn_SR(SOCK_SNMP_AGENT))
	{
		case SOCK_UDP :
			if ( (len = getSn_RX_RSR(SOCK_SNMP_AGENT)) > 0)
			{
				request_msg.len= recvfrom(SOCK_SNMP_AGENT, request_msg.buffer, len, svr_addr, &svr_port);
			}
			else
			{
				request_msg.len = 0;
			}

			if (request_msg.len > 0)
			{
#ifdef _SNMP_DEBUG_
				dumpCode((void *)"\r\n[Request]\r\n", (void *)"\r\n", request_msg.buffer, request_msg.len);
#endif
				// Initialize
				request_msg.index = 0;
				response_msg.index = 0;
				errorStatus = errorIndex = 0;
				memset(response_msg.buffer, 0x00, MAX_SNMPMSG_LEN);

				// Received message parsing and send response process
				if (parseSNMPMessage() != -1)
				{
					ret = sendto(SOCK_SNMP_AGENT, response_msg.buffer, response_msg.index, svr_addr, svr_port);
					
					//printf("sent \r\n");
					if(ret < 0)
               {
                  //printf("S %d: sendto error. %d\r\n",SOCK_SNMP_AGENT,ret);
                  return ret;
               }
				}

#ifdef _SNMP_DEBUG_
				
				printf("S %d: sendto %d.%d.%d.%d port %d\r\n",SOCK_SNMP_AGENT,svr_addr[0],svr_addr[1],svr_addr[2],svr_addr[3],svr_port);
				dumpCode((void *)"\r\n[Response]\r\n", (void *)"\r\n", response_msg.buffer, response_msg.index);
				sendto(SOCK_SNMP_AGENT, response_msg.buffer, response_msg.index, svr_addr, svr_port);
				sendto(4,response_msg.buffer, response_msg.index, svr_addr, svr_port);
				//sendto(SOCK_UDPS,serverPacket,NTP_PACKET_SIZE,destip,destport);
				printf("sent \r\n");
#endif
			}
			break;

		case SOCK_CLOSED :
			if((ret = socket(SOCK_SNMP_AGENT, Sn_MR_UDP, PORT_SNMP_AGENT, 0x00)) != SOCK_SNMP_AGENT)
				return ret;
#ifdef _SNMP_DEBUG_
			printf(" Socket[%d] UDP Socket for SNMP Agent, port [%d]\r\n", SOCK_SNMP_AGENT, PORT_SNMP_AGENT);
#endif
			break;

		default :
			break;
	}


	return 1;
}
*/

//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
//		printf("RTC_Update_NOW, Time set!\r\n");	
//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
	
	
	//  if(setTimeNow == 1)
//	{
//		setTimeNow = 0;
//		RTC_Update();
//	}
typedef struct{

  uint8_t work_mode;
	uint8_t rs458_connection;
	uint8_t ntp_client_status;
	uint8_t ethernet_connection;
	uint8_t rtc_status; 
	uint8_t sync_status; 
} slave_status;
extern slave_status slave_clock;

/*
Thi thoang LED treo ko ro ly do, reset neu nghi ngo
*/	
void resetInitLED(void)
{
	#ifdef SLAVE_MATRIX
	up7_matrix_init();
	line2_matrix_init();
#endif
#ifndef SLAVE_MATRIX	
	display_init_check();
#endif	
}

void ntp_client_run(void)
{
	/* dong bo NTP
		Chu ky dong bo : synchronizePeriod
		synchronized_ntp = 0 => tien hanh dong bo
*/
		
//		if(synchronized_ntp != 0)
//		{
//			return;
//		}
		
		SNTP_run();
		
		
}


				//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
				//HAL_UART_Transmit(&huart2, aRxBuffer, 20, 100);
				//main_message_handle();
				
				//saved = 1;
				//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
				//printf("aRxBuffer %s; \r\n",aRxBuffer);
//Xu ly ban tin tu mach main
void main_message_handle(void)
{//=> Ban tin GPS: $GPS034007060819AA10	;$$GPS091259280422AA10
	if((aRxBuffer[0] =='$')&((aRxBuffer[1] =='G')|(aRxBuffer[1] =='g'))&((aRxBuffer[2] =='P')|(aRxBuffer[2] =='p'))&((aRxBuffer[3] =='S')|(aRxBuffer[3] =='s')))
	{
		
		#ifndef SLAVE_MATRIX	
		MAX7219_SendAddrDat(0x07,0x00);//Tat het den => dan dc ban tin
		#endif	
		#ifdef SLAVE_MATRIX
		 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2|PC3_Pin, GPIO_PIN_SET);//Tat het den => dan dc ban tin
		#endif
		
		//Ban tin dung, cap nhat du lieu
		//If there is not GPS master message, no time on webserver
		days 		= 10*convert_atoi(aRxBuffer[10])+convert_atoi(aRxBuffer[11]);
		months 	= 10*convert_atoi(aRxBuffer[12])+convert_atoi(aRxBuffer[13]);
		years 	= 10*convert_atoi(aRxBuffer[14])+convert_atoi(aRxBuffer[15]);
		hours 	= 10*convert_atoi(aRxBuffer[4])+convert_atoi(aRxBuffer[5])  ;//UTC
		minutes = 10*convert_atoi(aRxBuffer[6])+convert_atoi(aRxBuffer[7]);
		seconds = 10*convert_atoi(aRxBuffer[8])+convert_atoi(aRxBuffer[9]);
		/*
		load_line2(hours,minutes,seconds,1);
		scan_5down();
		*/
		
		/*Cap nhap thoi gian NTP*/
//		currtime.tm_year = 100+ years;//100+10*convert_atoi(aRxBuffer[14])+convert_atoi(aRxBuffer[15]);//In fact: 2000+xxx-1900
//		currtime.tm_mon  = months-1;//10*convert_atoi(aRxBuffer[12])+convert_atoi(aRxBuffer[13])-1;
//		currtime.tm_mday = days;//10*convert_atoi(aRxBuffer[10])+convert_atoi(aRxBuffer[11]);
//		
//		currtime.tm_sec  = seconds;//10*convert_atoi(aRxBuffer[8])+convert_atoi(aRxBuffer[9]);
//		currtime.tm_min  = minutes;//10*convert_atoi(aRxBuffer[6])+convert_atoi(aRxBuffer[7]);
//		currtime.tm_hour = hours;//10*convert_atoi(aRxBuffer[4])+convert_atoi(aRxBuffer[5]);
//		
//		
		
		haveSignalFromRS485 = HAVE_SIGNAL;
		timeOutLostSignal = 10;
//	  if(count_Stable_signal < STABE_NUMBER) count_Stable_signal++; 
//		if(count_Stable_signal >= STABE_NUMBER) 
//			{
//				stableSignal = SIGNAL_FROM_MASTER_OK;
//			}
			
		if((aRxBuffer[16]=='A') || (aRxBuffer[17]=='A') )
						{ 
							slave_clock.sync_status = GREEN;
						}
		if((aRxBuffer[16]=='V') && (aRxBuffer[17]=='V') )				
		  slave_clock.sync_status = BOTH;
							
		if(timeSaveRS485_to_RTC == 1)
		{
			//seconds++;
			timeSaveRS485_to_RTC = 60*5;
			//sync rs485 time to RTC 
			ghids(14,0);//1HZ out SQW
			ghids(DS_SECOND_REG,seconds);
			ghids(DS_MIN_REG,minutes);
			ghids(DS_HOUR_REG,hours);
			//ghids(DS_DAY_REG,6);
			ghids(DS_DATE_REG,days);
			ghids(DS_MONTH_REG,months);
			ghids(DS_YEAR_REG,years);
		}
		
		
		
	}
	else control();
}

/*
Control
*/
void control(void)
{
	uint8_t tmp[4];
	if(unlock_config == 1)
	{//set all config here!!
		if((aRxBuffer[0] =='I')&&(aRxBuffer[1] =='P')&&(aRxBuffer[7] =='.')&&(aRxBuffer[11] =='.')&&(aRxBuffer[15] =='.'))
		{
			//IP: 192.168.222.252\r\n;IP: 092.068.022.052\r\n
			if((aRxBuffer[4]>'2') || (aRxBuffer[4]<'0')) return;
			if((aRxBuffer[8]>'2') || (aRxBuffer[8]<'0')) return;	
			if((aRxBuffer[12]>'2') || (aRxBuffer[12]<'0')) return;
			if((aRxBuffer[16]>'2') || (aRxBuffer[16]<'0')) return;
			
			if((aRxBuffer[5]>'9') || (aRxBuffer[5]<'0')) return;
			if((aRxBuffer[9]>'9') || (aRxBuffer[9]<'0')) return;	
			if((aRxBuffer[13]>'9') || (aRxBuffer[13]<'0')) return;
			if((aRxBuffer[17]>'9') || (aRxBuffer[17]<'0')) return;
			
			if((aRxBuffer[6]>'9') || (aRxBuffer[6]<'0')) return;
			if((aRxBuffer[10]>'9') || (aRxBuffer[10]<'0')) return;	
			if((aRxBuffer[14]>'9') || (aRxBuffer[14]<'0')) return;
			if((aRxBuffer[18]>'9') || (aRxBuffer[18]<'0')) return;
			
			myipWIZNETINFO.ip[0] = 100*(aRxBuffer[4]-'0')  + 10*(aRxBuffer[5]-'0')  + (aRxBuffer[6]-'0');
			myipWIZNETINFO.ip[1] = 100*(aRxBuffer[8]-'0')  + 10*(aRxBuffer[9]-'0')  + (aRxBuffer[10]-'0');
			myipWIZNETINFO.ip[2] = 100*(aRxBuffer[12]-'0') + 10*(aRxBuffer[13]-'0') + (aRxBuffer[14]-'0');
			myipWIZNETINFO.ip[3] = 100*(aRxBuffer[16]-'0') + 10*(aRxBuffer[17]-'0') + (aRxBuffer[18]-'0');
			
			#ifdef DebugEnable
			printf("New IP: %d.%d.%d.%d\r\n",myipWIZNETINFO.ip[0],myipWIZNETINFO.ip[1],myipWIZNETINFO.ip[2],myipWIZNETINFO.ip[3]);
			#endif
			HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
			
			//NVIC_SystemReset();
		}
		if((aRxBuffer[0] =='R')&&(aRxBuffer[1] =='E')&&(aRxBuffer[2] =='S')&&(aRxBuffer[3] =='E')&&(aRxBuffer[4] =='T'))
		{
			//Restart MCU
			NVIC_SystemReset();
		}
	}
	else
	{
		if((aRxBuffer[0] =='A')&&(aRxBuffer[1] =='T'))
		{
			//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
			//printf("AT on\r\n");
			//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
			unlock_config = 1;
		}
		else 
		{
			//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
			//printf("Linh tinh\r\n");
			//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
		}
	}
	
	
}

if((aRxBuffer[16]=='V') && (aRxBuffer[17]=='V') )	
	

#ifdef SLAVE_MATRIX
			load_line1(days,months,years);
			scan_7up();
			load_line2(hours,minutes,seconds,1);
			scan_5down();
#endif
#ifndef SLAVE_MATRIX
#ifdef SLAVE_WALL			
			day_display();
#endif
			console_display();
			console_blink();
#endif

/*
	t_days = timeinfo->tm_mday;
	t_months = 1+timeinfo->tm_mon;
	t_years = timeinfo->tm_year-100;
	t_hours = timeinfo->tm_hour;
	t_minutes = timeinfo->tm_min;
			*/
//	#ifdef NTP_CLIENT_DEBUG
//	HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
//	printf("faster clock! Loop delay %d, avg offset :%d,TIM4 %d\r\n",loop_delay,avg_offset_stable,TIM4->CNT);	
//	HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
//	#endif

int8_t SNTP_run_backUp(void)
{
	uint32_t ret;
	uint16_t size;
	uint32_t destip = 0;
	uint16_t destport;
	int32_t offset;
	switch(getSn_SR(SOCK_SNTP))
	{
	case SOCK_UDP:
		if ((size = getSn_RX_RSR(SOCK_SNTP)) > 0)
		{
			//loop_delay = fractionOfSecond;
			loop_delay = counterOf10Second;
			slave_clock.sync_status = GREEN;
			//if (size > 56) size = 56;	// if Rx data size is lager than TX_RX_MAX_BUF_SIZE
			recvfrom(SOCK_SNTP, ntpTimeServer_buf, 56, (uint8_t *)&destip, &destport);
			
			sec = (ntpTimeServer_buf[40]<<24) + (ntpTimeServer_buf[41]<<16) + (ntpTimeServer_buf[42]<<8) + ntpTimeServer_buf[43] ;
			//timenow = sec - seventyYears;
			Server_time = sec - seventyYears;
			//Phan le cua giay nhan dc
			micros_transmit = (ntpTimeServer_buf[44]<<24) + (ntpTimeServer_buf[45]<<16) + (ntpTimeServer_buf[46]<<8) + ntpTimeServer_buf[47] ;
			sec_frac = micros_transmit / usShift;
			sec_frac = sec_frac - 1;
			sec_frac = sec_frac / 100;

			
//			if( (Server_time - timenow >1) || (timenow >Server_time))
//			{
//				 printf("Khac %d s\r\n",Server_time - timenow);
//				//timenow = Server_time;
//				//sycn_RTC_Now();
//			}
//			else if(Server_time != timenow) 

				//printf("Khac %d s\r\n",Server_time - timenow);
			
				//printf("Server frac %d, my frac: %d\r\n",(int)sec_frac,fractionOfSecond);
				//tre truyen + nhan ban tin
				//printf("NTP received size:%d Delta %d\r\n",size,loop_delay);
			//printf("Loop delay %d\r\n",loop_delay);
			currtime.tm_year = 100+ years;
			currtime.tm_mon  = months-1;
			currtime.tm_mday = days;
			currtime.tm_sec  = seconds;
			currtime.tm_min  = minutes;
			currtime.tm_hour = hours;
			timenow = mktime(&currtime);
			//printf("timenow: %d\r\n",timenow);
			
			offset = 10000*(Server_time - timenow) + (int)sec_frac + (loop_delay)/2 - fractionOfSecond;
			//printf("Tre thoi gian giua server - device %d\r\n",offset);

			if(((offset>0)&&(offset < 1000)) || ((offset<0)&&((0-offset)<1000)))
			{
				printf("Loop delay %d, offset :%d\r\n",loop_delay,offset);
				slave_clock.sync_status = GREEN;
				return 0;
			}
			else if(timenow > Server_time) 
			{
				sycn_RTC_Now(Server_time);
				slave_clock.sync_status = GREEN;
				return 0;
			}
			else
			{
				timeinfo = localtime( &timenow );
				asctime(timeinfo);
				days = timeinfo->tm_mday;
				months = 1+timeinfo->tm_mon;
				years = timeinfo->tm_year-100;
				hours = timeinfo->tm_hour;
				minutes = timeinfo->tm_min;
				seconds = timeinfo->tm_sec + offset/10000 + 1;
				if(seconds > 59) return 0;
				setTimeNow = 1;
				fractionOfSecond = fractionOfSecond + offset%10000;
			}
			slave_clock.sync_status = GREEN;
			printf("2. Loop delay %d, offset :%d\r\n",loop_delay,offset);	
				
				
			
			//close(SOCK_SNTP);
			//printf("Closed SOCK NTP\r\n");
			//synchronizePeriod = 16;
			//synchronized_ntp = 1;
			//NTP_alreadySent  = 0;
			return 1;
		}
		
		if(	NTP_alreadySent == 0)
			{
				sendto(SOCK_SNTP,ntpmessage,48,ntpTimeServer_ip,123);
				//tSend = fractionOfSecond;
				slave_clock.sync_status = RED;
				counterOf10Second = 0;
				//printf("NTP sent\r\n");	
				NTP_alreadySent = 1;
				NTP_alreadySentTimeOut = 9;
				tSend = fractionOfSecond;
				return 0;
			}
			
			
			
		
		break;
	case SOCK_CLOSED:
		if((ret=socket(SOCK_SNTP,Sn_MR_UDP,sntp_port,0x00)) != SOCK_SNTP) return ret;
		printf(" Socket[%d] UDP Socket for SNTP client started at port [%d]\r\n", SOCK_SNTP, sntp_port);
		break;
	}
	// Return value
	// 0 - failed / 1 - success
	return 0;
}

//void wzn_event_handle(void)
//{
//	uint16_t ir = 0;
//	uint8_t sir = 0;
//	
//	
//	if (ctlwizchip(CW_GET_INTERRUPT, &ir) == -1) {
//		printf("Cannot get ir...");
//		return;
//	}
//	//set ngat voi tung socket!!!
//	if (ir & IK_SOCK_2) 
//		{
//				sir = getSn_IR(SOCK_SNTP);
//				
//				if ((sir & Sn_IR_RECV) > 0) {
//					/* Clear Sn_IR_RECV flag. */
//					setSn_IR(SOCK_SNTP, Sn_IR_RECV);
//					
//					return;
//				}
//				else if ((sir & Sn_IR_SENDOK) > 0) {
//					/* Clear Sn_IR_SENDOK flag. */
//					setSn_IR(SOCK_SNTP, Sn_IR_SENDOK);					
//				}
//		}
//}
#define fullconfigpageold			"<!DOCTYPE html>"\
"<html>"\
"<body><CENTER>"\
"<h3>IP config</h3>"\
"<FORM METHOD='POST' action='config.cgi'><br>"\
"<input type=\"text\" name=\"ip\" value=\"192.168.1.39\"> DeviveIP<br>"\
"<input type=\"text\" name=\"gw\" value=\"0.0.0.0\"> Getway<br>"\
"<input type=\"text\" name=\"sn\" value=\"255.255.255.0\"> Subnet <br>"\
"<input type=\"submit\" value=\"SAVE\">"\
"</form>"\
"</CENTER></body>"\
"</html>"\

#define configpage			"<!DOCTYPE html>"\
"<html>"\
	"<head>"\
	"<title>GPS Master clock configuration</title>"\
	"<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>"\
		"<style>"\
			"body{font-family: Arial, Tahoma;font-size: 14px;}"\
			"#main{width: 500px;border: 1px solid #6495ED;padding: 0;margin-left: auto;margin-right: auto;}"\
			"#head{border: 1px solid #CDCDCD;height: 70px;background-color:#008800;color:white;overflow: hidden;font-weight: bold;font-size:30px;text-align:center;margin-bottom:5px;}"\
			"#foot{background-color: #0099FF;border:1px solid #ccc;color:black;font-size:15px;height: 20px;overflow: hidden;padding-top: 5px;padding-bottom: 5px;text-align:center;}"\
 			"h1{color:black;font-size:18px;text-align:center;}"\
			"#net_info_general{min-height: 150px;border: 1px solid #CDCDCD;float:left;margin-bottom:5px;background-color: #F5F5DC;}"\
			"#list_head{background-color:#FFCC99;height: 40px;width: 100%;display: inline-block;font-size: 18px;}"\
			".tab{font-family:Arial; width: 498px; align : center; border: 0px; cellpadding:5px; cellspacing: 0px; height:50px;}"\
		"</style>"\
	"</head>"\
	"<body>"\
		"<div id='main'>"\
			"<div id='head'>GPS MASTER CLOCK CONFIGURATION</div>"\
			"<div id='net_info_general'>"\
				"<div id ='list_head'>"\
					"<h1>Network information</h1>"\
				"</div>"\
				"<div>"\
					"<FORM METHOD='POST' action='config.cgi'>"\
						"<table type ='table' class='tab'>"\
							"<tbody>"\
								"<tr>"\
									"<td>IP adress:&nbsp</td>"\
									"<td colspan='2'><input id='txtip' name='ip' type='text' value='192.168.1.39'/></td>"\
								"</tr>"\
								"<tr>"\
									"<td>Subnet mask:&nbsp</td>"\
									"<td colspan='2'><input id='txtsn' name='sn' type='text' value='255.255.255.0'/></td>"\
								"</tr>"\
								"<tr>"\
									"<td></td>"\
									"<td colspan='2'>"\
										"<input type='submit' class ='btn' name='bt_setup' value='Save' id='bt_setup'>"\
									"</td>"\
								"</tr>"\
							"</tbody>"\
						"</table>"\
					"</FORM>"\
				"</div>"\
			"</div>"\
			"<div id = 'foot'>"\
				"Powered by <a style='color: #000000; font-weight: bold; text-decoration: none'>Air Traffic Technical Company - <span style='color:red'>A</span><span style='color: blue;'>TTECH</span></a>"\
			"</div>"\
		"</div>"\
	"</body>"\
"</html>"

#ifdef unknown

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void make_json_serial_data(uint8_t * buf, uint16_t * len)
{
	uint8_t * buf_ptr;
	//buf_ptr = getUSART1buf();
	*len = sprintf((char *)buf,"getRs232DataCallback({\"data\":\"%s\"});", buf_ptr); // serial buffer

	// Serial data buffer clear
	//USART1_flush();
}


/************************************************************************************************/
uint8_t predefined_get_cgi_processor(uint8_t * uri_name, uint8_t * buf, uint16_t * len)
{
		uint8_t ret = 1;	// ret = 1 means 'uri_name' matched
	uint8_t cgibuf[14] = {0, };
	int8_t cgi_dio = -1;
	int8_t cgi_ain = -1;

	uint8_t i;

	if(strcmp((const char *)uri_name, "todo.cgi") == 0)
	{
		// to do
		;//make_json_todo(buf, len);
	}
	else if(strcmp((const char *)uri_name, "get_netinfo.cgi") == 0)
	{
		make_json_netinfo2(buf, len);
	}
	else
	{
		// get_dio0.cgi ~ get_dio15.cgi
		for(i = 0; i < DIOn; i++)
		{
			memset(cgibuf, 0x00, 14);
			sprintf((char *)cgibuf, "get_dio%d.cgi", i);
			if(strcmp((const char *)uri_name, (const char *)cgibuf) == 0)
			{
				make_json_dio(buf, len, i);
				cgi_dio = i;
				break;
			}
		}

		if(cgi_dio < 0)
		{
			// get_ain0.cgi ~ get_ain5.cgi (A0 - A5), get_ain6.cgi for on-board potentiometer / Temp.Sensor
			for(i = 0; i < AINn; i++)
			{
				memset(cgibuf, 0x00, 14);
				sprintf((char *)cgibuf, "get_ain%d.cgi", i);
				if(strcmp((const char *)uri_name, (const char *)cgibuf) == 0)
				{
					make_json_ain(buf, len, i);
					cgi_ain = i;
					break;
				}
			}
		}

		if((cgi_dio < 0) && (cgi_ain < 0)) ret = 0;
	}

	return ret;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pre-defined Get CGI functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void make_json_dio(uint8_t * buf, uint16_t * len, uint8_t pin)
{
	uint8_t pin_state 	= 1;//Chip_GPIO_GetPinState(LPC_GPIO, dio_ports[pin], dio_pins[pin]);
	uint8_t pin_dir 	= 2;//Chip_GPIO_GetPinDIR(LPC_GPIO, dio_ports[pin], dio_pins[pin]);

	*len = sprintf((char *)buf, "DioCallback({\"dio_p\":\"%d\",\
											\"dio_s\":\"%d\",\
											\"dio_d\":\"%d\"\
											});",
											pin,					// Digital io pin number
											pin_state,				// Digital io status
											pin_dir					// Digital io directions
											);
}

void make_json_ain(uint8_t * buf, uint16_t * len, uint8_t pin)
{
	*len = sprintf((char *)buf, "AinCallback({\"ain_p\":\"%d\",\
											\"ain_v\":\"%d\"\
											});",
											pin,					// ADC input pin number
											6		// get_ADC_val(pin), ADC input value
											);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pre-defined Set CGI functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int8_t set_diodir(uint8_t * uri)
{
	uint8_t * param;
	uint8_t pin = 0, val = 0;

	//printf("uri :%s\r\n",uri);
	
	if((param = get_http_param_value((char *)uri, "pin"))) // GPIO; D0 ~ D15
	{
		pin = (uint8_t)ATOI(param, 10);
		if(pin > 15) return -1;

		if((param = get_http_param_value((char *)uri, "val")))  // Direction; NotUsed/Input/Output
		{
			val = (uint8_t)ATOI(param, 10);
			if(val > Output) val = Output;
		}
	}

	if(val == Input) 		;//Chip_GPIO_SetPinDIRInput(LPC_GPIO, dio_ports[pin], dio_pins[pin]);	// Input
	else 					;//Chip_GPIO_SetPinDIROutput(LPC_GPIO, dio_ports[pin], dio_pins[pin]); // Output

	return pin;
}



int8_t set_diostate(uint8_t * uri)
{
	uint8_t * param;
	uint8_t pin = 0, val = 0;

	//printf("uri :%s\r\n",uri);
	//Toi day trong uri van chua nguyen ban tin
	if((param = get_http_param_value((char *)uri, "pin"))) // GPIO; D0 ~ D15
	{
		pin = (uint8_t)ATOI(param, 10);
		if(pin > 15) return -1;

		if((param = get_http_param_value((char *)uri, "val")))  // State; high(on)/low(off)
		{
			val = (uint8_t)ATOI(param, 10);
			if(val > On) val = On;
		}

		if(val == On) 		;//Chip_GPIO_SetPinState(LPC_GPIO, dio_ports[pin], dio_pins[pin], true); 	// High
		else				;//Chip_GPIO_SetPinState(LPC_GPIO, dio_ports[pin], dio_pins[pin], false);	// Low
	}

	return pin;
}
#endif
//	}
//	else
//	{
//		*len = sprintf((char *)buf, "{\"mac\":\"%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\",\"txtip\":\"%d.%d.%d.%d\",\"gw\":\"%d.%d.%d.%d\",\"txtsn\":\"%d.%d.%d.%d\",\"dns\":\"%d.%d.%d.%d\",\"dhcp\":\"%d\",\"txtdays\":\"%d\",\"txtmonths\":\"%d\",\"txtyears\":\"%d\",\"txthours\":\"%d\",\"txtminutes\":\"%d\",\"txtseconds\":\"%d\",\"txtgps01\":\"%s\",\"txtgps02\":\"%s\",\"txtpower01\":\"%s\",\"txtpower02\":\"%s\"}",
//											netinfo.mac[0], netinfo.mac[1], netinfo.mac[2], netinfo.mac[3], netinfo.mac[4], netinfo.mac[5],
//											netinfo.ip[0], netinfo.ip[1], netinfo.ip[2], netinfo.ip[3],
//											netinfo.gw[0], netinfo.gw[1], netinfo.gw[2], netinfo.gw[3],
//											netinfo.sn[0], netinfo.sn[1], netinfo.sn[2], netinfo.sn[3],
//											netinfo.dns[0], netinfo.dns[1], netinfo.dns[2], netinfo.dns[3],
//											netinfo.dhcp,
//											days,
//											months,
//											years,
//											hours,
//											minutes,
//											seconds,
//											gps1_stt?"ON":"OFF",
//											gps2_stt?"ON":"OFF",
//											power1_stt?"ON":"OFF",
//											power2_stt?"ON":"OFF"
//											);
//	}		

	// DHCP: 1 - Static, 2 - DHCP
//	if(lostSignal == LOST_GPS_MASTER)
//	{

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t predefined_set_cgi_processor(uint8_t * uri_name, uint8_t * uri, uint8_t * buf, uint16_t * len)
{
	uint8_t ret = 1;	// ret = '1' means 'uri_name' matched
//	uint8_t val = 0;

//	if(strcmp((const char *)uri_name, "set_diodir.cgi") == 0)
//	{
//		//val = set_diodir(uri);
//		*len = sprintf((char *)buf, "%d", val);
//	}
////	else if(strcmp((const char *)uri_name, "set_diostate.cgi") == 0)
////	{
////		//val = set_diostate(uri);
////		*len = sprintf((char *)buf, "%d", val);
////	}
//	else
//	{
//		ret = 0;
//		//printf("predefined_set_cgi_processor not found\r\n");
//	}

	return ret;
}

//Sai khac qua 1s
			if(Server_time > timenow)
			{
				if((Server_time - timenow) > 1)
				{
					

					printf("Offset BIG %d s\r\n",Server_time - timenow);

					sycn_RTC_Now(Server_time);
					return 1;
				}
			}
			else if(Server_time < timenow)
			{
				if((timenow - Server_time) > 1)
				{

					printf("Offset BIG %d s\r\n",timenow- Server_time);

					sycn_RTC_Now(Server_time);
					return 1;
				}
			}
			
			
//Neu RTC cham qua
uint8_t sycn_RTC_slow_old(time_t x)
{
			timeinfo = localtime( &x );
			asctime(timeinfo);
	
	    seconds = timeinfo->tm_sec + 1;
			if(seconds > 56) return 0;
	
			days = timeinfo->tm_mday;
			months = 1+timeinfo->tm_mon;
			years = timeinfo->tm_year-100;
			hours = timeinfo->tm_hour;
			minutes = timeinfo->tm_min;
			
			RTC_Update();
	    //printf("Save low RTC!\r\n");
	    
			return 1;
}


//Neu RTC nhanh qua
uint8_t sycn_RTC_fast_new(time_t x)
{
			timeinfo = localtime( &x );
			asctime(timeinfo);
	
	    seconds = timeinfo->tm_sec;
			if(seconds > 56) return 0;
	
			days = timeinfo->tm_mday;
			months = 1+timeinfo->tm_mon;
			years = timeinfo->tm_year-100;
			hours = timeinfo->tm_hour;
			minutes = timeinfo->tm_min;
			
			RTC_Update();
	    
	    
			return 1;
}


if((offset >= lastOffset) && ((offset - lastOffset) <= 999))
			{
				//printf("offset > lastOffset %d\r\n",offset- lastOffset);
				TimeServerStable++;
				avg_offset = avg_offset + offset;
			}
			else if((offset < lastOffset) && ((lastOffset - offset) <= 999))
			{
				//printf("offset < lastOffset %d\r\n",lastOffset - offset);
				TimeServerStable++;
				avg_offset = avg_offset + offset;
			}
			
			
			
//Do sai lech offset/last offset < 100ms => tin hieu co ve on dinh => dong bo
			if( ((offset >= lastOffset) && ((offset - lastOffset) <= 999))  || ((offset < lastOffset) && ((lastOffset - offset) <= 999)) )
			{
				//printf("offset > lastOffset %d\r\n",offset- lastOffset);
				TimeServerStable++;
				avg_offset = avg_offset + offset;
			}
			
			
			//Do sai lech Time Server > 100ms => tin hieu co ve vo van, tu tu roi tinh!!!
			if((offset >= lastOffset) && ((offset - lastOffset) > 999))
			{
				#ifdef NTP_CLIENT_DEBUG
				
				printf("Unstable offset > lastOffset %d\r\n",offset- lastOffset);
				
				#endif
				TimeServerStable = 0;
				avg_offset = 0;
			}
			else if((offset < lastOffset) && ((lastOffset - offset) > 999))
			{
				#ifdef NTP_CLIENT_DEBUG
				
				printf("Unstable offset < lastOffset %d\r\n",lastOffset - offset);
				
				#endif
				TimeServerStable = 0;
				avg_offset = 0;
			}
			
			
			
			
			//Bat dau chinh gio
			if(avg_offset_stable>=0)// Dong ho bi cham
			{
				if(avg_offset_stable < 999) //Sai lech duoi 99.9ms
					{
						#ifdef NTP_CLIENT_DEBUG
						printf("Time server on dinh! AVG: %d\r\n",avg_offset_stable);
				#endif
						return 1;// gio chuan ko phai chinh
					}
					
				else 
					{
						sycn_RTC_slow(Server_time);
						printf("Slower clock! Loop delay %d, avg offset :%d\r\n",loop_delay,avg_offset_stable);	

					}
			}
			else if(avg_offset_stable<0)//Dong ho nhanh
			{
				if(avg_offset_stable > -999) //Sai lech duoi 99.9ms
					{
						printf("Time server on dinh! AVG: %d\r\n",avg_offset_stable);
						return 1;// gio chuan ko phai chinh	
					}
					
				else 
				{
					sycn_RTC_fast(Server_time);
					

					printf("faster clock! Loop delay %d, avg offset :%d\r\n",loop_delay,avg_offset_stable);	

				}
			}
			
			
			
			ntpmessage[0] = 0xE3;   // LI, Version, Mode
	  ntpmessage[1] = 0;     // Stratum, or type of clock
	  ntpmessage[2] = 6;     // Polling Interval
	  ntpmessage[3] = 0;  // Peer Clock Precision
	  // 8 bytes of zero for Root Delay & Root Dispersion
	  ntpmessage[12]  = 49;
	  ntpmessage[13]  = 0x4E;
	  ntpmessage[14]  = 49;
	  ntpmessage[15]  = 52;
			
			
void RTC_Update_NOW(void)
	{
		ghids(14,0);//1HZ out SQW
		ghids(DS_SECOND_REG,	t_seconds);
		ghids(DS_MIN_REG,			t_minutes);
		ghids(DS_HOUR_REG,		t_hours);
		//ghids(DS_DAY_REG,2);
		ghids(DS_DATE_REG,		t_days);
		ghids(DS_MONTH_REG,		t_months);
		ghids(DS_YEAR_REG,		t_years);
		
	
		laythoigian();
		
//		currtime.tm_year = 100+ years;
//		currtime.tm_mon  = months-1;
//		currtime.tm_mday = days;
//		currtime.tm_sec  = seconds;
//		currtime.tm_min  = minutes;
//		currtime.tm_hour = hours;
//		timenow = mktime(&currtime);
		
		//printf("Time set!!!!\r\n");
	}	


//		if(testLinhTinh == 5)
//		{
//			
//			if(fractionOfSecond > 300	)
//			{
//				testLinhTinh = 0; // Moi giay kiem tra 1 lan 
//				if(slave_clock.rs458_connection == CONNECTED)
//				{
//					//Neu thoi gian khac nhau !
//					if(server_second != seconds) 
//					{
//						//time2SaveRTC = 6;
//						#ifdef OverTheAir
//						send_debug_message = 1;
//						mysize = sprintf(udp_message,"Sai gio %d/local %d, frac %d, tx %d\r\n",server_second,seconds,fractionOfSecond,tx);
//						#endif
//						
//					}
//				}
//					
//				
//			}
//		}	