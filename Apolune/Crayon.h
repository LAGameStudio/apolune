/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once
#include "moremath.h"
#include "Strings.h"
#include "Randoms.h"
#include "BinaryFile.h"

class Blends;

#define SetColorf(c)   glColor4f( c.rf, c.gf, c.bf, c.af )

#define TOTAL_COLORS 525+16+16+4+10+8704 /* = 9275 */
#define TOTAL_COLORS_AND_GLASS TOTAL_COLORS+5 /* 9280 */

enum Colors {
 cyan=0, chocolate=1, /* white */ alabaster=2, black=3, clear=4, red255=5, green255=6, blue255=7,
 hotPink=8, aliceBlue=9, blue=10, green=11, cadetBlue=12, orangeRed=13, brown=14, indigo=15,
 thistle=16, darkRed=17, darkSalmon=18, salmon=19, purple=20, deepSkyBlue=21, steelBlue=22, slateBlue=23, yellow=24,
 orange=25, bloodRed=26, skyBlue=27, rosyBrown=28, violet=29, darkChocolate=30, crimson=31, chartreuse=32,
 lightSkyBlue=33, beige=34, bisque=35, fireBrick=36, gold=37, OldLace=38, Linen=39, turquoise=40,
 gray=41, cornflowerBlue=42, charcoal=43,
  /* Browns and yellows */
 cornsilk=44, beige2=45, cream=46, lemonChiffon=47, paleGoldenrod=48, wheat=49, dandelion=50, mustard=51, mediumGoldenrod=52,
 corn=53, lemonYellow=54, goldenYellow=55, schoolBusYellow=56, golden=57, sunglow=58, tangerineYellow=59, saffron=60,
 goldenPoppy=61, amber=62, selectiveYellow=63, macNcheese=64, sandyBrown=65, desertSand=66,
 paleGold=67, brass=68, californiaGold=69, vegasGold=70, oldGold=71, metallicGold=72, goldenrod=73,
 satinSheenGold=74, buddhaGold=75, peru=76, roseGold=77, ochre=78,
 darkGoldenrod=79, sandyTaupe=80, goldenBrown=81, rawUmber=82,
  /* Pinks */
 palePink=83, pigPink=84, pink=85, cottonCandy=86, babyPink=87, mediumPink=88,
 cherryBlossomPink=89, nadashikoPink=90, carnationPink=91, tickleMePink=92,
 raspberryPink=93, darkPink=94, brinkPink=95, puce=96, pastelPink=97,
 lightSalmonPink=98, salmonPink=99, mediumSalmon=100, salmon2=101,
 darkSalmon2=102, coralPink=103, lightCoral=104, lightCarminePink=105,
 carminePink=106, deepCarminePink=107, vermilion=108,
  /* Tans, Oranges and Reds */
 lightChocolate=109, terraCotta=110, tomato=111, coralRed=112, carmineRed=113, redOrange2=114, electricVermilion=115,
 orangeRed3=116, coquelicot=117, scarlet=118, rED=119, pigmentRed=120, lust=121, crimson2=122, ruby=123, raspberry=124,
 razzmatazz=125, cerise=126, alizarinCrimson=127, electricCrimson=128, americanRose=129, radicalRed=130, amaranth=131,
 richCarmine=132, fireEngineRed=133, harvardCrimson=134, cardinal=135,
  /* Purply Reds */
 brightMaroon=136, richMaroon=137, raspberryRose=138, raspberryGlace=139, deepCarmine=140, crimsonGlory=141,
 brightTyrianPurple=142, mediumCarmine=143, mahogany=144, venetianRed=145, persianRed=146, brickRed=147,
 darkTerraCotta=148, chestnut=149,
  /* Browns, Reds, Burgundies and Auburns */
 fuzzyWuzzyBrown=150, deepChestnut=151, redwood=152, mediumChocolate=153, darkChestnut=154, cafeAuLait=155, russet=156,
 rust=157, burntUmber=158, vividBurgundy=159, redBrown=160, fireBrick2=161, carnelian=162, upsdellRed=163, rufous=164,
 cognac=165, teaRed=166, sangria=167, vividAuburn=168, burgundy=169, mediumTyrianPurple=170, carmine=171, darkRed2=172,
 faluRed=173, maroon=174, tuscanRed=175, bole=176, chocolate3=177, darkChocolate2=178, auburn=179, persianPlum=180,
 darkRaspberry=181, tyrianPurple=182, caputMortuum=183, coffee=184,
  /* Greens */
 olivineGreen=185, oliveDrab=186, oLIVE=187, darkOliveGreen=188, lavenderMist=189,
  /* Magentas, Purples, Lilacs */
 lightMauve=190, mauve=191, thistle2=192, lilac=193, lightOrchid=194, palePlum=195,
 lavenderMagenta=196, orchid=197, heliotrope=198, psychedelicPurple=199, electricPurple=200,
 pontiff=201, x11Purple=202, darkOrchid=203, deepPurple=204, internetPurple=205,
  /* Blues, Indigos, Violets */
 deepIndigo=206, purpleHeart=207, royalPurple=208, genericPurple=209, pigmentIndigo=210,
 darkIndigo=211, vulgarPurple=212, persianIndigo=213, ultramarine=214, persianBlue=215,
 electricUltramarine=216, hanPurple=217, westminster=218, indigo2=219, violet2=220, pigmentViolet=221,
 violetPurple=222, lavender=223, purpleMountainMajesty=224, purpleYam=225, mediumPurple=226,
 amethyst=227, deepLilac=228, mediumOrchid=229, deepFuchsia=230, operaMauve=231,
 mauveTaupe=232, mulberry=233, paleRedViolet=234, cabaret=235, royalFuchsia=236,
 byzantine=237, fandango=238, mediumRedViolet=239, redViolet=240, hibiscus=241,
 brightTyrianPurple2=242, mediumTyrianPurple2=243, amaranthPurple=244, nightShadz=245,
 royalHeath=246, jazzberryJam=247, amaranthDeepPurple=248, redVioletEggplant=249, disco=250,
 plum=251, violetEggplant=252, pURPLE=253, byzantium=254, palatinatePurple=255, tokyoPurple=256,
 halayaUbe=257, pansyPurple=258, nightclub=259, tyrianPurple2=260, cosmic=261, persianPlum2=262,
 eggplant=263, darkByzantium=264, purpleTaupe=265, blackberry=266,
  /* Beiges */
 beige3=267, blanchedAlmond=268, bananaMania=269, moccasin=270, peachYellow=271, wheat2=272,
 bisque2=273, navajoWhite=274, lightKhaki=275, paleGoldenrod2=276, buff=277, flax=278, zinnwaldite=279,
 desertSand2=280, ecru=281, tan2=282, khaki=283, darkKhaki=284, rosyBrown2=285, terraCotta2=286,
 bittersweet=287, sandyBrown2=288, goldenrod2=289, brass2=290, paleTaupe=291, antiqueBrass=292,
 bronze=293, ochre2=294, copper=295, darkGoldenrod2=296, sandyTaupe2=297, darkTan=298, shadow=299,
 beaver=300, frenchBeige=301, paleBrown=302, peru2=303, cocoaBean=304, tenne=305, burntOrange=306,
 mahogany2=307, darkTerraCotta2=308, chestnut2=309, fuzzyWuzzyBrown2=310, deepChestnut2=311, darkChestnut2=312,
 mediumBrown=313, sienna=314, bROWN=315, saddleBrown=316, rusty=317, idaho=318, burntUmber2=319,
 redBrown2=320, burgundy2=321, maroon2=322, auburn2=323, rawUmber2=324, pullmanBrown=325, darkBrown=326,
 sepia=327, bole2=328, mediumTaupe=329, taupe=330, cafe=331, liver=332, bistre=333, sealBrown=334,
 seashell=335, oldLace=336, antiqueWhite=337, blanchedAlmond2=338, champagne=339,
  /* Others (Orange/Yellow/Brown/Pink) */
 bananaMania2=340, moccasin2=341, bisque3=342, navajoWhite2=343, peachPuff=344, peach=345, deepPeach=346, peachOrange=347,
 brightApricot=348, apricot=349, melon=350, macaroniAndCheese=351, sandyBrown3=352, lightSalmon=353,
 darkSalmon3=354, mediumSalmon2=355, coral=356, salmon3=357, coralPink2=358, lightCoral2=359, bittersweet2=360,
 terraCotta3=361, teaRose=362, vividTangerine=363, atomicTangerine=364, orangePeel=365, orange2=366,
 deepSaffron=367, darkOrange=368, tangerine=369, oRANGE=370, automotiveAmber=371, pumpkin=372,
 richOrange=373, safetyOrange=374, cadmiumOrange=375, persimmon=376, tangelo=377, vermilion2=378,
 redOrange3=379, electricVermilion2=380, orangeRed2=381, internationalOrange=382, portlandOrange=383,
 ultraOrange=384, cherry=385, deepCarrotOrange=386, carrotOrange=387, neonCarrot=388, gamboge=389,
 ochre3=390, bronze2=391, copper2=392, paleBrown2=393, antiqueBrass2=394, peru3=395, cocoa=396,
 tenne2=397, burntOrange2=398, mahogany3=399, darkTerraCotta3=400, darkCoral=401, mediumBrown2=402,
 sienna2=403, brown2=404, saddleBrown2=405, rust2=406, russet2=407, redBrown3=408, cognacBrown=409,
 burntUmber3=410, pullmanBrown2=411, darkBrown2=412, mediumTaupe2=413, taupe2=414,
  /* Greenish blues */
 electricUltramarine2=415,
 mediumNavyBlue=416, mediumBlue=417, azure=418, pigmentBlue=419, ultramarine2=420, darkBlue=421,
 navy=422, persianIndigo2=423, stpattyBlue=424, midnightBlue=425, sapphire=426, paleSilver=427,
 silver=428, aliceBlue2=429, bubbles=430, lightCyan=431, paleTurquoise=432, paleRobinEggBlue=433,
 aquamarine=434, aquamarineBlue=435, babyBlue=436, electricBlue=437, cyan2=438, turquoiseBlue=439,
 brightTurquoise=440, turquoise2=441, mayaBlue=442, capri=443, processCyan=444, brightCerulean=445,
 deepTurquoise=446, robinEggBlue=447, mediumTurquoise=448, mediumAquamarine=449, lightSeaGreen=450,
 cadetBlue2=451, viridian=452, pineGreen=453, darkCyan=454, teal=455, tealBlue=456, bondiBlue=457,
 richElectricBlue=458, cerulean=459, steelBlue2=460, mediumElectricBlue=461, darkElectricBlue=462,
 midnightGreen=463, palePink2=464, pigPink2=465, pINK=466, cottonCandy2=467, babyPink2=468, mediumPink2=469,
 cherryBlossomPink2=470, nadashikoPink2=471, carnationPink2=472, lavenderPink=473, lavenderRose=474,
 lightFuchsiaPink=475, persianPink=476, tickleMePink2=477, mauvelous=478, amaranthPink=479,
 paleAmaranthPink=480, pastelPink2=481, puce2=482, paleVioletRed=483, lightCarminePink2=484,
 lightCoral3=485, coralPink3=486, salmon4=487, lightSalmonPink2=488, salmonPink2=489, pinkOrange=490, coral2=491,
 carnation=492, carminePink2=493,  deepCarminePink2=494, wildWatermelon=495, brinkPink2=496, darkPink2=497,
 cranberry=498, frenchRose=499, violetRed=500, hotPink2=501, brilliantRose=502, raspberryPink2=503,
 wildStrawberry=504, brightPink=505, deepPink=506, cerisePink=507, cerise2=508, tyrianPink=509,
 vividCerise=510, amaranthCerise=511, deepCerise=512, hollywoodCerise=513, persianRose=514,
 shockingPink=515, razzleDazzleRose=516, fuchsiaPink=517, ultraPink=518, pinkFlamingo=519, magenta=520,
 hotMagenta=521, amaranthMagenta=522, skyMagenta=523, fandango2=524,
 /* ANSI and extended terminal colors */
 ANSIgrey=525,       ANSIred=526,
 ANSIgreen=527,      ANSIamber=528,
 ANSIblue=529,       ANSImagenta=530,
 ANSIcyan=531,       ANSIboldgrey=532,
 ANSIboldred=533,    ANSIboldgreen=534,
 ANSIboldblue=535,   ANSIboldmagenta=536,
 ANSIwhite=537,      ANSIbackcyan=538,
 ANSIbackred=539,    ANSIbackgrey=540,
 ANSImutedgreen=541, ANSImutedyellow=542,
 ANSImutedred=543,   ANSImutedblue=544,
 ANSImutedgreen2=545,ANSIaltred=546,
 ANSIaltmagenta=547, ANSIaltyellow=548,
 ANSIaltblue=549,    ANSIaltwhite=550,
 ANSIaltbrown=551,   ANSIaltgreen=552,
 ANSIaltgrey=553,    ANSIaltblack=554,
 ANSIaltcyan=555,    ANSImonogreen=556,
 ANSImonoamber=557,  ANSImsxamber=558,
 c64blue=559, c64cyan=560,
 pullmanGreen=561, omahaOrange=562, imitationGold=563, dupontGray=564, vermillion=565,
 mineralRed=566, glacierGreen=567, wfeYellow=568, bigSkyBlue=569, gnGray=570,
  /* Glasses */
 redGlass=571, greenGlass=572, blueGlass=573, magentaGlass=574,

 // Generated rgbFFF and rgbFFF# where 0-9 is 0-90%
rgb000=575,rgb001=576,rgb002=577,rgb003=578,rgb004=579,rgb005=580,rgb006=581,rgb007=582,rgb008=583,rgb009=584,rgb00A=585,rgb00B=586,rgb00C=587,rgb00D=588,rgb00E=589,rgb00F=590,rgb010=591,rgb011=592,rgb012=593,rgb013=594,rgb014=595,rgb015=596,rgb016=597,rgb017=598,rgb018=599,rgb019=600,rgb01A=601,rgb01B=602,rgb01C=603,rgb01D=604,rgb01E=605,rgb01F=606,rgb020=607,rgb021=608,rgb022=609,rgb023=610,rgb024=611,rgb025=612,rgb026=613,rgb027=614,rgb028=615,rgb029=616,rgb02A=617,rgb02B=618,rgb02C=619,rgb02D=620,rgb02E=621,rgb02F=622,rgb030=623,rgb031=624,rgb032=625,rgb033=626,rgb034=627,rgb035=628,rgb036=629,rgb037=630,rgb038=631,rgb039=632,rgb03A=633,rgb03B=634,rgb03C=635,rgb03D=636,rgb03E=637,rgb03F=638,rgb040=639,rgb041=640,rgb042=641,rgb043=642,rgb044=643,rgb045=644,rgb046=645,rgb047=646,rgb048=647,rgb049=648,rgb04A=649,rgb04B=650,rgb04C=651,rgb04D=652,rgb04E=653,rgb04F=654,rgb050=655,rgb051=656,rgb052=657,rgb053=658,rgb054=659,rgb055=660,rgb056=661,rgb057=662,rgb058=663,rgb059=664,rgb05A=665,rgb05B=666,rgb05C=667,rgb05D=668,rgb05E=669,rgb05F=670,rgb060=671,rgb061=672,rgb062=673,rgb063=674,rgb064=675,rgb065=676,rgb066=677,rgb067=678,rgb068=679,rgb069=680,rgb06A=681,rgb06B=682,rgb06C=683,rgb06D=684,rgb06E=685,rgb06F=686,rgb070=687,rgb071=688,rgb072=689,rgb073=690,rgb074=691,rgb075=692,rgb076=693,rgb077=694,rgb078=695,rgb079=696,rgb07A=697,rgb07B=698,rgb07C=699,rgb07D=700,rgb07E=701,rgb07F=702,rgb080=703,rgb081=704,rgb082=705,rgb083=706,rgb084=707,rgb085=708,rgb086=709,rgb087=710,rgb088=711,rgb089=712,rgb08A=713,rgb08B=714,rgb08C=715,rgb08D=716,rgb08E=717,rgb08F=718,rgb090=719,rgb091=720,rgb092=721,rgb093=722,rgb094=723,rgb095=724,rgb096=725,rgb097=726,rgb098=727,rgb099=728,rgb09A=729,rgb09B=730,rgb09C=731,rgb09D=732,rgb09E=733,rgb09F=734,rgb0A0=735,rgb0A1=736,rgb0A2=737,rgb0A3=738,rgb0A4=739,rgb0A5=740,rgb0A6=741,rgb0A7=742,rgb0A8=743,rgb0A9=744,rgb0AA=745,rgb0AB=746,rgb0AC=747,rgb0AD=748,rgb0AE=749,rgb0AF=750,rgb0B0=751,rgb0B1=752,rgb0B2=753,rgb0B3=754,rgb0B4=755,rgb0B5=756,rgb0B6=757,rgb0B7=758,rgb0B8=759,rgb0B9=760,rgb0BA=761,rgb0BB=762,rgb0BC=763,rgb0BD=764,rgb0BE=765,rgb0BF=766,rgb0C0=767,rgb0C1=768,rgb0C2=769,rgb0C3=770,rgb0C4=771,rgb0C5=772,rgb0C6=773,rgb0C7=774,rgb0C8=775,rgb0C9=776,rgb0CA=777,rgb0CB=778,rgb0CC=779,rgb0CD=780,rgb0CE=781,rgb0CF=782,
rgb0D0=783,rgb0D1=784,rgb0D2=785,rgb0D3=786,rgb0D4=787,rgb0D5=788,rgb0D6=789,rgb0D7=790,rgb0D8=791,rgb0D9=792,rgb0DA=793,rgb0DB=794,rgb0DC=795,rgb0DD=796,rgb0DE=797,rgb0DF=798,rgb0E0=799,rgb0E1=800,rgb0E2=801,rgb0E3=802,rgb0E4=803,rgb0E5=804,rgb0E6=805,rgb0E7=806,rgb0E8=807,rgb0E9=808,rgb0EA=809,rgb0EB=810,rgb0EC=811,rgb0ED=812,rgb0EE=813,rgb0EF=814,rgb0F0=815,rgb0F1=816,rgb0F2=817,rgb0F3=818,rgb0F4=819,rgb0F5=820,rgb0F6=821,rgb0F7=822,rgb0F8=823,rgb0F9=824,rgb0FA=825,rgb0FB=826,rgb0FC=827,rgb0FD=828,rgb0FE=829,rgb0FF=830,rgb100=831,rgb101=832,rgb102=833,rgb103=834,rgb104=835,rgb105=836,rgb106=837,rgb107=838,rgb108=839,rgb109=840,rgb10A=841,rgb10B=842,rgb10C=843,rgb10D=844,rgb10E=845,rgb10F=846,rgb110=847,rgb111=848,rgb112=849,rgb113=850,rgb114=851,rgb115=852,rgb116=853,rgb117=854,rgb118=855,rgb119=856,rgb11A=857,rgb11B=858,rgb11C=859,rgb11D=860,rgb11E=861,rgb11F=862,rgb120=863,rgb121=864,rgb122=865,rgb123=866,rgb124=867,rgb125=868,rgb126=869,rgb127=870,rgb128=871,rgb129=872,rgb12A=873,rgb12B=874,rgb12C=875,rgb12D=876,rgb12E=877,rgb12F=878,rgb130=879,rgb131=880,rgb132=881,rgb133=882,rgb134=883,rgb135=884,rgb136=885,rgb137=886,rgb138=887,rgb139=888,rgb13A=889,rgb13B=890,rgb13C=891,rgb13D=892,rgb13E=893,rgb13F=894,rgb140=895,rgb141=896,rgb142=897,rgb143=898,rgb144=899,rgb145=900,rgb146=901,rgb147=902,rgb148=903,rgb149=904,rgb14A=905,rgb14B=906,rgb14C=907,rgb14D=908,rgb14E=909,rgb14F=910,rgb150=911,rgb151=912,rgb152=913,rgb153=914,rgb154=915,rgb155=916,rgb156=917,rgb157=918,rgb158=919,rgb159=920,rgb15A=921,rgb15B=922,rgb15C=923,rgb15D=924,rgb15E=925,rgb15F=926,rgb160=927,rgb161=928,rgb162=929,rgb163=930,rgb164=931,rgb165=932,rgb166=933,rgb167=934,rgb168=935,rgb169=936,rgb16A=937,rgb16B=938,rgb16C=939,rgb16D=940,rgb16E=941,rgb16F=942,rgb170=943,rgb171=944,rgb172=945,rgb173=946,rgb174=947,rgb175=948,rgb176=949,rgb177=950,rgb178=951,rgb179=952,rgb17A=953,rgb17B=954,rgb17C=955,rgb17D=956,rgb17E=957,rgb17F=958,rgb180=959,rgb181=960,rgb182=961,rgb183=962,rgb184=963,rgb185=964,rgb186=965,rgb187=966,rgb188=967,rgb189=968,rgb18A=969,rgb18B=970,rgb18C=971,rgb18D=972,rgb18E=973,rgb18F=974,rgb190=975,rgb191=976,rgb192=977,rgb193=978,rgb194=979,rgb195=980,rgb196=981,rgb197=982,rgb198=983,rgb199=984,rgb19A=985,rgb19B=986,rgb19C=987,rgb19D=988,rgb19E=989,rgb19F=990,
rgb1A0=991,rgb1A1=992,rgb1A2=993,rgb1A3=994,rgb1A4=995,rgb1A5=996,rgb1A6=997,rgb1A7=998,rgb1A8=999,rgb1A9=1000,rgb1AA=1001,rgb1AB=1002,rgb1AC=1003,rgb1AD=1004,rgb1AE=1005,rgb1AF=1006,rgb1B0=1007,rgb1B1=1008,rgb1B2=1009,rgb1B3=1010,rgb1B4=1011,rgb1B5=1012,rgb1B6=1013,rgb1B7=1014,rgb1B8=1015,rgb1B9=1016,rgb1BA=1017,rgb1BB=1018,rgb1BC=1019,rgb1BD=1020,rgb1BE=1021,rgb1BF=1022,rgb1C0=1023,rgb1C1=1024,rgb1C2=1025,rgb1C3=1026,rgb1C4=1027,rgb1C5=1028,rgb1C6=1029,rgb1C7=1030,rgb1C8=1031,rgb1C9=1032,rgb1CA=1033,rgb1CB=1034,rgb1CC=1035,rgb1CD=1036,rgb1CE=1037,rgb1CF=1038,rgb1D0=1039,rgb1D1=1040,rgb1D2=1041,rgb1D3=1042,rgb1D4=1043,rgb1D5=1044,rgb1D6=1045,rgb1D7=1046,rgb1D8=1047,rgb1D9=1048,rgb1DA=1049,rgb1DB=1050,rgb1DC=1051,rgb1DD=1052,rgb1DE=1053,rgb1DF=1054,rgb1E0=1055,rgb1E1=1056,rgb1E2=1057,rgb1E3=1058,rgb1E4=1059,rgb1E5=1060,rgb1E6=1061,rgb1E7=1062,rgb1E8=1063,rgb1E9=1064,rgb1EA=1065,rgb1EB=1066,rgb1EC=1067,rgb1ED=1068,rgb1EE=1069,rgb1EF=1070,rgb1F0=1071,rgb1F1=1072,rgb1F2=1073,rgb1F3=1074,rgb1F4=1075,rgb1F5=1076,rgb1F6=1077,rgb1F7=1078,rgb1F8=1079,rgb1F9=1080,rgb1FA=1081,rgb1FB=1082,rgb1FC=1083,rgb1FD=1084,rgb1FE=1085,rgb1FF=1086,rgb200=1087,rgb201=1088,rgb202=1089,rgb203=1090,rgb204=1091,rgb205=1092,rgb206=1093,rgb207=1094,rgb208=1095,rgb209=1096,rgb20A=1097,rgb20B=1098,rgb20C=1099,rgb20D=1100,rgb20E=1101,rgb20F=1102,rgb210=1103,rgb211=1104,rgb212=1105,rgb213=1106,rgb214=1107,rgb215=1108,rgb216=1109,rgb217=1110,rgb218=1111,rgb219=1112,rgb21A=1113,rgb21B=1114,rgb21C=1115,rgb21D=1116,rgb21E=1117,rgb21F=1118,rgb220=1119,rgb221=1120,rgb222=1121,rgb223=1122,rgb224=1123,rgb225=1124,rgb226=1125,rgb227=1126,rgb228=1127,rgb229=1128,rgb22A=1129,rgb22B=1130,rgb22C=1131,rgb22D=1132,rgb22E=1133,rgb22F=1134,rgb230=1135,rgb231=1136,rgb232=1137,rgb233=1138,rgb234=1139,rgb235=1140,rgb236=1141,rgb237=1142,rgb238=1143,rgb239=1144,rgb23A=1145,
rgb23B=1146,rgb23C=1147,rgb23D=1148,rgb23E=1149,rgb23F=1150,rgb240=1151,rgb241=1152,rgb242=1153,rgb243=1154,rgb244=1155,rgb245=1156,rgb246=1157,rgb247=1158,rgb248=1159,rgb249=1160,rgb24A=1161,rgb24B=1162,rgb24C=1163,rgb24D=1164,rgb24E=1165,rgb24F=1166,rgb250=1167,rgb251=1168,rgb252=1169,rgb253=1170,rgb254=1171,rgb255=1172,rgb256=1173,rgb257=1174,rgb258=1175,rgb259=1176,rgb25A=1177,rgb25B=1178,rgb25C=1179,rgb25D=1180,rgb25E=1181,rgb25F=1182,rgb260=1183,rgb261=1184,rgb262=1185,rgb263=1186,rgb264=1187,rgb265=1188,rgb266=1189,rgb267=1190,rgb268=1191,rgb269=1192,rgb26A=1193,rgb26B=1194,rgb26C=1195,rgb26D=1196,rgb26E=1197,rgb26F=1198,rgb270=1199,rgb271=1200,rgb272=1201,rgb273=1202,rgb274=1203,rgb275=1204,rgb276=1205,rgb277=1206,rgb278=1207,rgb279=1208,rgb27A=1209,rgb27B=1210,rgb27C=1211,rgb27D=1212,rgb27E=1213,rgb27F=1214,rgb280=1215,rgb281=1216,rgb282=1217,rgb283=1218,rgb284=1219,rgb285=1220,rgb286=1221,rgb287=1222,rgb288=1223,rgb289=1224,rgb28A=1225,rgb28B=1226,rgb28C=1227,rgb28D=1228,rgb28E=1229,rgb28F=1230,rgb290=1231,rgb291=1232,rgb292=1233,rgb293=1234,rgb294=1235,rgb295=1236,rgb296=1237,rgb297=1238,rgb298=1239,rgb299=1240,rgb29A=1241,rgb29B=1242,rgb29C=1243,rgb29D=1244,rgb29E=1245,rgb29F=1246,rgb2A0=1247,rgb2A1=1248,rgb2A2=1249,rgb2A3=1250,rgb2A4=1251,rgb2A5=1252,rgb2A6=1253,rgb2A7=1254,rgb2A8=1255,rgb2A9=1256,rgb2AA=1257,rgb2AB=1258,rgb2AC=1259,rgb2AD=1260,rgb2AE=1261,rgb2AF=1262,rgb2B0=1263,rgb2B1=1264,rgb2B2=1265,rgb2B3=1266,rgb2B4=1267,rgb2B5=1268,rgb2B6=1269,rgb2B7=1270,rgb2B8=1271,rgb2B9=1272,rgb2BA=1273,rgb2BB=1274,rgb2BC=1275,rgb2BD=1276,rgb2BE=1277,rgb2BF=1278,rgb2C0=1279,rgb2C1=1280,rgb2C2=1281,rgb2C3=1282,rgb2C4=1283,rgb2C5=1284,rgb2C6=1285,rgb2C7=1286,rgb2C8=1287,rgb2C9=1288,rgb2CA=1289,rgb2CB=1290,rgb2CC=1291,rgb2CD=1292,rgb2CE=1293,rgb2CF=1294,rgb2D0=1295,rgb2D1=1296,rgb2D2=1297,rgb2D3=1298,rgb2D4=1299,rgb2D5=1300,rgb2D6=1301,rgb2D7=1302,rgb2D8=1303,rgb2D9=1304,rgb2DA=1305,rgb2DB=1306,rgb2DC=1307,rgb2DD=1308,rgb2DE=1309,rgb2DF=1310,rgb2E0=1311,rgb2E1=1312,rgb2E2=1313,rgb2E3=1314,rgb2E4=1315,rgb2E5=1316,rgb2E6=1317,rgb2E7=1318,rgb2E8=1319,rgb2E9=1320,rgb2EA=1321,rgb2EB=1322,rgb2EC=1323,rgb2ED=1324,rgb2EE=1325,rgb2EF=1326,rgb2F0=1327,rgb2F1=1328,rgb2F2=1329,rgb2F3=1330,rgb2F4=1331,rgb2F5=1332,rgb2F6=1333,rgb2F7=1334,rgb2F8=1335,rgb2F9=1336,
rgb2FA=1337,rgb2FB=1338,rgb2FC=1339,rgb2FD=1340,rgb2FE=1341,rgb2FF=1342,rgb300=1343,rgb301=1344,rgb302=1345,rgb303=1346,rgb304=1347,rgb305=1348,rgb306=1349,rgb307=1350,rgb308=1351,rgb309=1352,rgb30A=1353,rgb30B=1354,rgb30C=1355,rgb30D=1356,rgb30E=1357,rgb30F=1358,rgb310=1359,rgb311=1360,rgb312=1361,rgb313=1362,rgb314=1363,rgb315=1364,rgb316=1365,rgb317=1366,rgb318=1367,rgb319=1368,rgb31A=1369,rgb31B=1370,rgb31C=1371,rgb31D=1372,rgb31E=1373,rgb31F=1374,rgb320=1375,rgb321=1376,rgb322=1377,rgb323=1378,rgb324=1379,rgb325=1380,rgb326=1381,rgb327=1382,rgb328=1383,rgb329=1384,rgb32A=1385,rgb32B=1386,rgb32C=1387,rgb32D=1388,rgb32E=1389,rgb32F=1390,rgb330=1391,rgb331=1392,rgb332=1393,rgb333=1394,rgb334=1395,rgb335=1396,rgb336=1397,rgb337=1398,rgb338=1399,rgb339=1400,rgb33A=1401,rgb33B=1402,rgb33C=1403,rgb33D=1404,rgb33E=1405,rgb33F=1406,rgb340=1407,rgb341=1408,rgb342=1409,rgb343=1410,rgb344=1411,rgb345=1412,rgb346=1413,rgb347=1414,rgb348=1415,rgb349=1416,rgb34A=1417,rgb34B=1418,rgb34C=1419,rgb34D=1420,rgb34E=1421,rgb34F=1422,rgb350=1423,rgb351=1424,rgb352=1425,rgb353=1426,rgb354=1427,rgb355=1428,rgb356=1429,rgb357=1430,rgb358=1431,rgb359=1432,rgb35A=1433,rgb35B=1434,rgb35C=1435,rgb35D=1436,rgb35E=1437,rgb35F=1438,rgb360=1439,rgb361=1440,rgb362=1441,rgb363=1442,rgb364=1443,rgb365=1444,rgb366=1445,rgb367=1446,rgb368=1447,rgb369=1448,rgb36A=1449,rgb36B=1450,rgb36C=1451,rgb36D=1452,rgb36E=1453,rgb36F=1454,rgb370=1455,rgb371=1456,rgb372=1457,rgb373=1458,rgb374=1459,rgb375=1460,rgb376=1461,rgb377=1462,rgb378=1463,rgb379=1464,rgb37A=1465,rgb37B=1466,rgb37C=1467,rgb37D=1468,rgb37E=1469,rgb37F=1470,rgb380=1471,rgb381=1472,rgb382=1473,rgb383=1474,rgb384=1475,rgb385=1476,rgb386=1477,rgb387=1478,rgb388=1479,rgb389=1480,rgb38A=1481,rgb38B=1482,rgb38C=1483,rgb38D=1484,rgb38E=1485,rgb38F=1486,rgb390=1487,rgb391=1488,rgb392=1489,rgb393=1490,rgb394=1491,rgb395=1492,rgb396=1493,rgb397=1494,rgb398=1495,rgb399=1496,rgb39A=1497,rgb39B=1498,rgb39C=1499,rgb39D=1500,rgb39E=1501,rgb39F=1502,rgb3A0=1503,rgb3A1=1504,rgb3A2=1505,rgb3A3=1506,rgb3A4=1507,rgb3A5=1508,rgb3A6=1509,rgb3A7=1510,rgb3A8=1511,rgb3A9=1512,rgb3AA=1513,rgb3AB=1514,rgb3AC=1515,rgb3AD=1516,rgb3AE=1517,rgb3AF=1518,rgb3B0=1519,rgb3B1=1520,rgb3B2=1521,rgb3B3=1522,rgb3B4=1523,rgb3B5=1524,rgb3B6=1525,rgb3B7=1526,rgb3B8=1527,
rgb3B9=1528,rgb3BA=1529,rgb3BB=1530,rgb3BC=1531,rgb3BD=1532,rgb3BE=1533,rgb3BF=1534,rgb3C0=1535,rgb3C1=1536,rgb3C2=1537,rgb3C3=1538,rgb3C4=1539,rgb3C5=1540,rgb3C6=1541,rgb3C7=1542,rgb3C8=1543,rgb3C9=1544,rgb3CA=1545,rgb3CB=1546,rgb3CC=1547,rgb3CD=1548,rgb3CE=1549,rgb3CF=1550,rgb3D0=1551,rgb3D1=1552,rgb3D2=1553,rgb3D3=1554,rgb3D4=1555,rgb3D5=1556,rgb3D6=1557,rgb3D7=1558,rgb3D8=1559,rgb3D9=1560,rgb3DA=1561,rgb3DB=1562,rgb3DC=1563,rgb3DD=1564,rgb3DE=1565,rgb3DF=1566,rgb3E0=1567,rgb3E1=1568,rgb3E2=1569,rgb3E3=1570,rgb3E4=1571,rgb3E5=1572,rgb3E6=1573,rgb3E7=1574,rgb3E8=1575,rgb3E9=1576,rgb3EA=1577,rgb3EB=1578,rgb3EC=1579,rgb3ED=1580,rgb3EE=1581,rgb3EF=1582,rgb3F0=1583,rgb3F1=1584,rgb3F2=1585,rgb3F3=1586,rgb3F4=1587,rgb3F5=1588,rgb3F6=1589,rgb3F7=1590,rgb3F8=1591,rgb3F9=1592,rgb3FA=1593,rgb3FB=1594,rgb3FC=1595,rgb3FD=1596,rgb3FE=1597,rgb3FF=1598,rgb400=1599,rgb401=1600,rgb402=1601,rgb403=1602,rgb404=1603,rgb405=1604,rgb406=1605,rgb407=1606,rgb408=1607,rgb409=1608,rgb40A=1609,rgb40B=1610,rgb40C=1611,rgb40D=1612,rgb40E=1613,rgb40F=1614,rgb410=1615,rgb411=1616,rgb412=1617,rgb413=1618,rgb414=1619,rgb415=1620,rgb416=1621,rgb417=1622,rgb418=1623,rgb419=1624,rgb41A=1625,rgb41B=1626,rgb41C=1627,rgb41D=1628,rgb41E=1629,rgb41F=1630,rgb420=1631,rgb421=1632,rgb422=1633,rgb423=1634,rgb424=1635,rgb425=1636,rgb426=1637,rgb427=1638,rgb428=1639,rgb429=1640,rgb42A=1641,rgb42B=1642,rgb42C=1643,rgb42D=1644,rgb42E=1645,rgb42F=1646,rgb430=1647,rgb431=1648,rgb432=1649,rgb433=1650,rgb434=1651,rgb435=1652,rgb436=1653,rgb437=1654,rgb438=1655,rgb439=1656,rgb43A=1657,rgb43B=1658,rgb43C=1659,rgb43D=1660,rgb43E=1661,rgb43F=1662,rgb440=1663,rgb441=1664,rgb442=1665,rgb443=1666,rgb444=1667,rgb445=1668,rgb446=1669,rgb447=1670,rgb448=1671,rgb449=1672,rgb44A=1673,rgb44B=1674,rgb44C=1675,rgb44D=1676,rgb44E=1677,rgb44F=1678,rgb450=1679,rgb451=1680,rgb452=1681,rgb453=1682,rgb454=1683,
rgb455=1684,rgb456=1685,rgb457=1686,rgb458=1687,rgb459=1688,rgb45A=1689,rgb45B=1690,rgb45C=1691,rgb45D=1692,rgb45E=1693,rgb45F=1694,rgb460=1695,rgb461=1696,rgb462=1697,rgb463=1698,rgb464=1699,rgb465=1700,rgb466=1701,rgb467=1702,rgb468=1703,rgb469=1704,rgb46A=1705,rgb46B=1706,rgb46C=1707,rgb46D=1708,rgb46E=1709,rgb46F=1710,rgb470=1711,rgb471=1712,rgb472=1713,rgb473=1714,rgb474=1715,rgb475=1716,rgb476=1717,rgb477=1718,rgb478=1719,rgb479=1720,rgb47A=1721,rgb47B=1722,rgb47C=1723,rgb47D=1724,rgb47E=1725,rgb47F=1726,rgb480=1727,rgb481=1728,rgb482=1729,rgb483=1730,rgb484=1731,rgb485=1732,rgb486=1733,rgb487=1734,rgb488=1735,rgb489=1736,rgb48A=1737,rgb48B=1738,rgb48C=1739,rgb48D=1740,rgb48E=1741,rgb48F=1742,rgb490=1743,rgb491=1744,rgb492=1745,rgb493=1746,rgb494=1747,rgb495=1748,rgb496=1749,rgb497=1750,rgb498=1751,rgb499=1752,rgb49A=1753,rgb49B=1754,rgb49C=1755,rgb49D=1756,rgb49E=1757,rgb49F=1758,rgb4A0=1759,rgb4A1=1760,rgb4A2=1761,rgb4A3=1762,rgb4A4=1763,rgb4A5=1764,rgb4A6=1765,rgb4A7=1766,rgb4A8=1767,rgb4A9=1768,rgb4AA=1769,rgb4AB=1770,rgb4AC=1771,rgb4AD=1772,rgb4AE=1773,rgb4AF=1774,rgb4B0=1775,rgb4B1=1776,rgb4B2=1777,rgb4B3=1778,rgb4B4=1779,rgb4B5=1780,rgb4B6=1781,rgb4B7=1782,rgb4B8=1783,rgb4B9=1784,rgb4BA=1785,rgb4BB=1786,rgb4BC=1787,rgb4BD=1788,rgb4BE=1789,rgb4BF=1790,rgb4C0=1791,rgb4C1=1792,rgb4C2=1793,rgb4C3=1794,rgb4C4=1795,rgb4C5=1796,rgb4C6=1797,rgb4C7=1798,rgb4C8=1799,rgb4C9=1800,rgb4CA=1801,rgb4CB=1802,rgb4CC=1803,rgb4CD=1804,rgb4CE=1805,rgb4CF=1806,rgb4D0=1807,rgb4D1=1808,rgb4D2=1809,rgb4D3=1810,rgb4D4=1811,rgb4D5=1812,rgb4D6=1813,rgb4D7=1814,rgb4D8=1815,rgb4D9=1816,rgb4DA=1817,rgb4DB=1818,rgb4DC=1819,rgb4DD=1820,rgb4DE=1821,rgb4DF=1822,rgb4E0=1823,rgb4E1=1824,rgb4E2=1825,rgb4E3=1826,rgb4E4=1827,rgb4E5=1828,rgb4E6=1829,rgb4E7=1830,rgb4E8=1831,rgb4E9=1832,rgb4EA=1833,rgb4EB=1834,rgb4EC=1835,rgb4ED=1836,rgb4EE=1837,rgb4EF=1838,rgb4F0=1839,rgb4F1=1840,rgb4F2=1841,rgb4F3=1842,rgb4F4=1843,rgb4F5=1844,rgb4F6=1845,rgb4F7=1846,rgb4F8=1847,rgb4F9=1848,rgb4FA=1849,rgb4FB=1850,rgb4FC=1851,rgb4FD=1852,rgb4FE=1853,rgb4FF=1854,rgb500=1855,rgb501=1856,rgb502=1857,rgb503=1858,rgb504=1859,rgb505=1860,rgb506=1861,rgb507=1862,rgb508=1863,rgb509=1864,rgb50A=1865,rgb50B=1866,rgb50C=1867,rgb50D=1868,rgb50E=1869,rgb50F=1870,rgb510=1871,rgb511=1872,rgb512=1873,rgb513=1874,
rgb514=1875,rgb515=1876,rgb516=1877,rgb517=1878,rgb518=1879,rgb519=1880,rgb51A=1881,rgb51B=1882,rgb51C=1883,rgb51D=1884,rgb51E=1885,rgb51F=1886,rgb520=1887,rgb521=1888,rgb522=1889,rgb523=1890,rgb524=1891,rgb525=1892,rgb526=1893,rgb527=1894,rgb528=1895,rgb529=1896,rgb52A=1897,rgb52B=1898,rgb52C=1899,rgb52D=1900,rgb52E=1901,rgb52F=1902,rgb530=1903,rgb531=1904,rgb532=1905,rgb533=1906,rgb534=1907,rgb535=1908,rgb536=1909,rgb537=1910,rgb538=1911,rgb539=1912,rgb53A=1913,rgb53B=1914,rgb53C=1915,rgb53D=1916,rgb53E=1917,rgb53F=1918,rgb540=1919,rgb541=1920,rgb542=1921,rgb543=1922,rgb544=1923,rgb545=1924,rgb546=1925,rgb547=1926,rgb548=1927,rgb549=1928,rgb54A=1929,rgb54B=1930,rgb54C=1931,rgb54D=1932,rgb54E=1933,rgb54F=1934,rgb550=1935,rgb551=1936,rgb552=1937,rgb553=1938,rgb554=1939,rgb555=1940,rgb556=1941,rgb557=1942,rgb558=1943,rgb559=1944,rgb55A=1945,rgb55B=1946,rgb55C=1947,rgb55D=1948,rgb55E=1949,rgb55F=1950,rgb560=1951,rgb561=1952,rgb562=1953,rgb563=1954,rgb564=1955,rgb565=1956,rgb566=1957,rgb567=1958,rgb568=1959,rgb569=1960,rgb56A=1961,rgb56B=1962,rgb56C=1963,rgb56D=1964,rgb56E=1965,rgb56F=1966,rgb570=1967,rgb571=1968,rgb572=1969,rgb573=1970,rgb574=1971,rgb575=1972,rgb576=1973,rgb577=1974,rgb578=1975,rgb579=1976,rgb57A=1977,rgb57B=1978,rgb57C=1979,rgb57D=1980,rgb57E=1981,rgb57F=1982,rgb580=1983,rgb581=1984,rgb582=1985,rgb583=1986,rgb584=1987,rgb585=1988,rgb586=1989,rgb587=1990,rgb588=1991,rgb589=1992,rgb58A=1993,rgb58B=1994,rgb58C=1995,rgb58D=1996,rgb58E=1997,rgb58F=1998,rgb590=1999,rgb591=2000,rgb592=2001,rgb593=2002,rgb594=2003,rgb595=2004,rgb596=2005,rgb597=2006,rgb598=2007,rgb599=2008,rgb59A=2009,rgb59B=2010,rgb59C=2011,rgb59D=2012,rgb59E=2013,rgb59F=2014,rgb5A0=2015,rgb5A1=2016,rgb5A2=2017,rgb5A3=2018,rgb5A4=2019,rgb5A5=2020,rgb5A6=2021,rgb5A7=2022,rgb5A8=2023,rgb5A9=2024,rgb5AA=2025,rgb5AB=2026,rgb5AC=2027,rgb5AD=2028,rgb5AE=2029,rgb5AF=2030,rgb5B0=2031,rgb5B1=2032,rgb5B2=2033,rgb5B3=2034,rgb5B4=2035,rgb5B5=2036,rgb5B6=2037,rgb5B7=2038,rgb5B8=2039,rgb5B9=2040,rgb5BA=2041,rgb5BB=2042,rgb5BC=2043,rgb5BD=2044,rgb5BE=2045,rgb5BF=2046,rgb5C0=2047,rgb5C1=2048,rgb5C2=2049,rgb5C3=2050,rgb5C4=2051,rgb5C5=2052,rgb5C6=2053,rgb5C7=2054,rgb5C8=2055,rgb5C9=2056,rgb5CA=2057,rgb5CB=2058,rgb5CC=2059,rgb5CD=2060,rgb5CE=2061,rgb5CF=2062,rgb5D0=2063,rgb5D1=2064,rgb5D2=2065,
rgb5D3=2066,rgb5D4=2067,rgb5D5=2068,rgb5D6=2069,rgb5D7=2070,rgb5D8=2071,rgb5D9=2072,rgb5DA=2073,rgb5DB=2074,rgb5DC=2075,rgb5DD=2076,rgb5DE=2077,rgb5DF=2078,rgb5E0=2079,rgb5E1=2080,rgb5E2=2081,rgb5E3=2082,rgb5E4=2083,rgb5E5=2084,rgb5E6=2085,rgb5E7=2086,rgb5E8=2087,rgb5E9=2088,rgb5EA=2089,rgb5EB=2090,rgb5EC=2091,rgb5ED=2092,rgb5EE=2093,rgb5EF=2094,rgb5F0=2095,rgb5F1=2096,rgb5F2=2097,rgb5F3=2098,rgb5F4=2099,rgb5F5=2100,rgb5F6=2101,rgb5F7=2102,rgb5F8=2103,rgb5F9=2104,rgb5FA=2105,rgb5FB=2106,rgb5FC=2107,rgb5FD=2108,rgb5FE=2109,rgb5FF=2110,rgb600=2111,rgb601=2112,rgb602=2113,rgb603=2114,rgb604=2115,rgb605=2116,rgb606=2117,rgb607=2118,rgb608=2119,rgb609=2120,rgb60A=2121,rgb60B=2122,rgb60C=2123,rgb60D=2124,rgb60E=2125,rgb60F=2126,rgb610=2127,rgb611=2128,rgb612=2129,rgb613=2130,rgb614=2131,rgb615=2132,rgb616=2133,rgb617=2134,rgb618=2135,rgb619=2136,rgb61A=2137,rgb61B=2138,rgb61C=2139,rgb61D=2140,rgb61E=2141,rgb61F=2142,rgb620=2143,rgb621=2144,rgb622=2145,rgb623=2146,rgb624=2147,rgb625=2148,rgb626=2149,rgb627=2150,rgb628=2151,rgb629=2152,rgb62A=2153,rgb62B=2154,rgb62C=2155,rgb62D=2156,rgb62E=2157,rgb62F=2158,rgb630=2159,rgb631=2160,rgb632=2161,rgb633=2162,rgb634=2163,rgb635=2164,rgb636=2165,rgb637=2166,rgb638=2167,rgb639=2168,rgb63A=2169,rgb63B=2170,rgb63C=2171,rgb63D=2172,rgb63E=2173,rgb63F=2174,rgb640=2175,rgb641=2176,rgb642=2177,rgb643=2178,rgb644=2179,rgb645=2180,rgb646=2181,rgb647=2182,rgb648=2183,rgb649=2184,rgb64A=2185,rgb64B=2186,rgb64C=2187,rgb64D=2188,rgb64E=2189,rgb64F=2190,rgb650=2191,rgb651=2192,rgb652=2193,rgb653=2194,rgb654=2195,rgb655=2196,rgb656=2197,rgb657=2198,rgb658=2199,rgb659=2200,rgb65A=2201,rgb65B=2202,rgb65C=2203,rgb65D=2204,rgb65E=2205,rgb65F=2206,rgb660=2207,rgb661=2208,rgb662=2209,rgb663=2210,rgb664=2211,rgb665=2212,rgb666=2213,rgb667=2214,rgb668=2215,rgb669=2216,rgb66A=2217,rgb66B=2218,rgb66C=2219,rgb66D=2220,rgb66E=2221,
rgb66F=2222,rgb670=2223,rgb671=2224,rgb672=2225,rgb673=2226,rgb674=2227,rgb675=2228,rgb676=2229,rgb677=2230,rgb678=2231,rgb679=2232,rgb67A=2233,rgb67B=2234,rgb67C=2235,rgb67D=2236,rgb67E=2237,rgb67F=2238,rgb680=2239,rgb681=2240,rgb682=2241,rgb683=2242,rgb684=2243,rgb685=2244,rgb686=2245,rgb687=2246,rgb688=2247,rgb689=2248,rgb68A=2249,rgb68B=2250,rgb68C=2251,rgb68D=2252,rgb68E=2253,rgb68F=2254,rgb690=2255,rgb691=2256,rgb692=2257,rgb693=2258,rgb694=2259,rgb695=2260,rgb696=2261,rgb697=2262,rgb698=2263,rgb699=2264,rgb69A=2265,rgb69B=2266,rgb69C=2267,rgb69D=2268,rgb69E=2269,rgb69F=2270,rgb6A0=2271,rgb6A1=2272,rgb6A2=2273,rgb6A3=2274,rgb6A4=2275,rgb6A5=2276,rgb6A6=2277,rgb6A7=2278,rgb6A8=2279,rgb6A9=2280,rgb6AA=2281,rgb6AB=2282,rgb6AC=2283,rgb6AD=2284,rgb6AE=2285,rgb6AF=2286,rgb6B0=2287,rgb6B1=2288,rgb6B2=2289,rgb6B3=2290,rgb6B4=2291,rgb6B5=2292,rgb6B6=2293,rgb6B7=2294,rgb6B8=2295,rgb6B9=2296,rgb6BA=2297,rgb6BB=2298,rgb6BC=2299,rgb6BD=2300,rgb6BE=2301,rgb6BF=2302,rgb6C0=2303,rgb6C1=2304,rgb6C2=2305,rgb6C3=2306,rgb6C4=2307,rgb6C5=2308,rgb6C6=2309,rgb6C7=2310,rgb6C8=2311,rgb6C9=2312,rgb6CA=2313,rgb6CB=2314,rgb6CC=2315,rgb6CD=2316,rgb6CE=2317,rgb6CF=2318,rgb6D0=2319,rgb6D1=2320,rgb6D2=2321,rgb6D3=2322,rgb6D4=2323,rgb6D5=2324,rgb6D6=2325,rgb6D7=2326,rgb6D8=2327,rgb6D9=2328,rgb6DA=2329,rgb6DB=2330,rgb6DC=2331,rgb6DD=2332,rgb6DE=2333,rgb6DF=2334,rgb6E0=2335,rgb6E1=2336,rgb6E2=2337,rgb6E3=2338,rgb6E4=2339,rgb6E5=2340,rgb6E6=2341,rgb6E7=2342,rgb6E8=2343,rgb6E9=2344,rgb6EA=2345,rgb6EB=2346,rgb6EC=2347,rgb6ED=2348,rgb6EE=2349,rgb6EF=2350,rgb6F0=2351,rgb6F1=2352,rgb6F2=2353,rgb6F3=2354,rgb6F4=2355,rgb6F5=2356,rgb6F6=2357,rgb6F7=2358,rgb6F8=2359,rgb6F9=2360,rgb6FA=2361,rgb6FB=2362,rgb6FC=2363,rgb6FD=2364,rgb6FE=2365,rgb6FF=2366,rgb700=2367,rgb701=2368,rgb702=2369,rgb703=2370,rgb704=2371,rgb705=2372,rgb706=2373,rgb707=2374,rgb708=2375,rgb709=2376,rgb70A=2377,rgb70B=2378,rgb70C=2379,rgb70D=2380,rgb70E=2381,rgb70F=2382,rgb710=2383,rgb711=2384,rgb712=2385,rgb713=2386,rgb714=2387,rgb715=2388,rgb716=2389,rgb717=2390,rgb718=2391,rgb719=2392,rgb71A=2393,rgb71B=2394,rgb71C=2395,rgb71D=2396,rgb71E=2397,rgb71F=2398,rgb720=2399,rgb721=2400,rgb722=2401,rgb723=2402,rgb724=2403,rgb725=2404,rgb726=2405,rgb727=2406,rgb728=2407,rgb729=2408,rgb72A=2409,rgb72B=2410,rgb72C=2411,rgb72D=2412,
rgb72E=2413,rgb72F=2414,rgb730=2415,rgb731=2416,rgb732=2417,rgb733=2418,rgb734=2419,rgb735=2420,rgb736=2421,rgb737=2422,rgb738=2423,rgb739=2424,rgb73A=2425,rgb73B=2426,rgb73C=2427,rgb73D=2428,rgb73E=2429,rgb73F=2430,rgb740=2431,rgb741=2432,rgb742=2433,rgb743=2434,rgb744=2435,rgb745=2436,rgb746=2437,rgb747=2438,rgb748=2439,rgb749=2440,rgb74A=2441,rgb74B=2442,rgb74C=2443,rgb74D=2444,rgb74E=2445,rgb74F=2446,rgb750=2447,rgb751=2448,rgb752=2449,rgb753=2450,rgb754=2451,rgb755=2452,rgb756=2453,rgb757=2454,rgb758=2455,rgb759=2456,rgb75A=2457,rgb75B=2458,rgb75C=2459,rgb75D=2460,rgb75E=2461,rgb75F=2462,rgb760=2463,rgb761=2464,rgb762=2465,rgb763=2466,rgb764=2467,rgb765=2468,rgb766=2469,rgb767=2470,rgb768=2471,rgb769=2472,rgb76A=2473,rgb76B=2474,rgb76C=2475,rgb76D=2476,rgb76E=2477,rgb76F=2478,rgb770=2479,rgb771=2480,rgb772=2481,rgb773=2482,rgb774=2483,rgb775=2484,rgb776=2485,rgb777=2486,rgb778=2487,rgb779=2488,rgb77A=2489,rgb77B=2490,rgb77C=2491,rgb77D=2492,rgb77E=2493,rgb77F=2494,rgb780=2495,rgb781=2496,rgb782=2497,rgb783=2498,rgb784=2499,rgb785=2500,rgb786=2501,rgb787=2502,rgb788=2503,rgb789=2504,rgb78A=2505,rgb78B=2506,rgb78C=2507,rgb78D=2508,rgb78E=2509,rgb78F=2510,rgb790=2511,rgb791=2512,rgb792=2513,rgb793=2514,rgb794=2515,rgb795=2516,rgb796=2517,rgb797=2518,rgb798=2519,rgb799=2520,rgb79A=2521,rgb79B=2522,rgb79C=2523,rgb79D=2524,rgb79E=2525,rgb79F=2526,rgb7A0=2527,rgb7A1=2528,rgb7A2=2529,rgb7A3=2530,rgb7A4=2531,rgb7A5=2532,rgb7A6=2533,rgb7A7=2534,rgb7A8=2535,rgb7A9=2536,rgb7AA=2537,rgb7AB=2538,rgb7AC=2539,rgb7AD=2540,rgb7AE=2541,rgb7AF=2542,rgb7B0=2543,rgb7B1=2544,rgb7B2=2545,rgb7B3=2546,rgb7B4=2547,rgb7B5=2548,rgb7B6=2549,rgb7B7=2550,rgb7B8=2551,rgb7B9=2552,rgb7BA=2553,rgb7BB=2554,rgb7BC=2555,rgb7BD=2556,rgb7BE=2557,rgb7BF=2558,rgb7C0=2559,rgb7C1=2560,rgb7C2=2561,rgb7C3=2562,rgb7C4=2563,rgb7C5=2564,rgb7C6=2565,rgb7C7=2566,rgb7C8=2567,rgb7C9=2568,rgb7CA=2569,rgb7CB=2570,rgb7CC=2571,rgb7CD=2572,rgb7CE=2573,rgb7CF=2574,rgb7D0=2575,rgb7D1=2576,rgb7D2=2577,rgb7D3=2578,rgb7D4=2579,rgb7D5=2580,rgb7D6=2581,rgb7D7=2582,rgb7D8=2583,rgb7D9=2584,rgb7DA=2585,rgb7DB=2586,rgb7DC=2587,rgb7DD=2588,rgb7DE=2589,rgb7DF=2590,rgb7E0=2591,rgb7E1=2592,rgb7E2=2593,rgb7E3=2594,rgb7E4=2595,rgb7E5=2596,rgb7E6=2597,rgb7E7=2598,rgb7E8=2599,rgb7E9=2600,rgb7EA=2601,rgb7EB=2602,rgb7EC=2603,
rgb7ED=2604,rgb7EE=2605,rgb7EF=2606,rgb7F0=2607,rgb7F1=2608,rgb7F2=2609,rgb7F3=2610,rgb7F4=2611,rgb7F5=2612,rgb7F6=2613,rgb7F7=2614,rgb7F8=2615,rgb7F9=2616,rgb7FA=2617,rgb7FB=2618,rgb7FC=2619,rgb7FD=2620,rgb7FE=2621,rgb7FF=2622,rgb800=2623,rgb801=2624,rgb802=2625,rgb803=2626,rgb804=2627,rgb805=2628,rgb806=2629,rgb807=2630,rgb808=2631,rgb809=2632,rgb80A=2633,rgb80B=2634,rgb80C=2635,rgb80D=2636,rgb80E=2637,rgb80F=2638,rgb810=2639,rgb811=2640,rgb812=2641,rgb813=2642,rgb814=2643,rgb815=2644,rgb816=2645,rgb817=2646,rgb818=2647,rgb819=2648,rgb81A=2649,rgb81B=2650,rgb81C=2651,rgb81D=2652,rgb81E=2653,rgb81F=2654,rgb820=2655,rgb821=2656,rgb822=2657,rgb823=2658,rgb824=2659,rgb825=2660,rgb826=2661,rgb827=2662,rgb828=2663,rgb829=2664,rgb82A=2665,rgb82B=2666,rgb82C=2667,rgb82D=2668,rgb82E=2669,rgb82F=2670,rgb830=2671,rgb831=2672,rgb832=2673,rgb833=2674,rgb834=2675,rgb835=2676,rgb836=2677,rgb837=2678,rgb838=2679,rgb839=2680,rgb83A=2681,rgb83B=2682,rgb83C=2683,rgb83D=2684,rgb83E=2685,rgb83F=2686,rgb840=2687,rgb841=2688,rgb842=2689,rgb843=2690,rgb844=2691,rgb845=2692,rgb846=2693,rgb847=2694,rgb848=2695,rgb849=2696,rgb84A=2697,rgb84B=2698,rgb84C=2699,rgb84D=2700,rgb84E=2701,rgb84F=2702,rgb850=2703,rgb851=2704,rgb852=2705,rgb853=2706,rgb854=2707,rgb855=2708,rgb856=2709,rgb857=2710,rgb858=2711,rgb859=2712,rgb85A=2713,rgb85B=2714,rgb85C=2715,rgb85D=2716,rgb85E=2717,rgb85F=2718,rgb860=2719,rgb861=2720,rgb862=2721,rgb863=2722,rgb864=2723,rgb865=2724,rgb866=2725,rgb867=2726,rgb868=2727,rgb869=2728,rgb86A=2729,rgb86B=2730,rgb86C=2731,rgb86D=2732,rgb86E=2733,rgb86F=2734,rgb870=2735,rgb871=2736,rgb872=2737,rgb873=2738,rgb874=2739,rgb875=2740,rgb876=2741,rgb877=2742,rgb878=2743,rgb879=2744,rgb87A=2745,rgb87B=2746,rgb87C=2747,rgb87D=2748,rgb87E=2749,rgb87F=2750,rgb880=2751,rgb881=2752,rgb882=2753,rgb883=2754,rgb884=2755,rgb885=2756,rgb886=2757,rgb887=2758,rgb888=2759,rgb889=2760,rgb88A=2761,rgb88B=2762,rgb88C=2763,rgb88D=2764,rgb88E=2765,rgb88F=2766,rgb890=2767,rgb891=2768,rgb892=2769,rgb893=2770,rgb894=2771,rgb895=2772,rgb896=2773,rgb897=2774,rgb898=2775,rgb899=2776,rgb89A=2777,rgb89B=2778,rgb89C=2779,rgb89D=2780,rgb89E=2781,rgb89F=2782,rgb8A0=2783,rgb8A1=2784,rgb8A2=2785,rgb8A3=2786,rgb8A4=2787,rgb8A5=2788,rgb8A6=2789,rgb8A7=2790,rgb8A8=2791,rgb8A9=2792,rgb8AA=2793,rgb8AB=2794,
rgb8AC=2795,rgb8AD=2796,rgb8AE=2797,rgb8AF=2798,rgb8B0=2799,rgb8B1=2800,rgb8B2=2801,rgb8B3=2802,rgb8B4=2803,rgb8B5=2804,rgb8B6=2805,rgb8B7=2806,rgb8B8=2807,rgb8B9=2808,rgb8BA=2809,rgb8BB=2810,rgb8BC=2811,rgb8BD=2812,rgb8BE=2813,rgb8BF=2814,rgb8C0=2815,rgb8C1=2816,rgb8C2=2817,rgb8C3=2818,rgb8C4=2819,rgb8C5=2820,rgb8C6=2821,rgb8C7=2822,rgb8C8=2823,rgb8C9=2824,rgb8CA=2825,rgb8CB=2826,rgb8CC=2827,rgb8CD=2828,rgb8CE=2829,rgb8CF=2830,rgb8D0=2831,rgb8D1=2832,rgb8D2=2833,rgb8D3=2834,rgb8D4=2835,rgb8D5=2836,rgb8D6=2837,rgb8D7=2838,rgb8D8=2839,rgb8D9=2840,rgb8DA=2841,rgb8DB=2842,rgb8DC=2843,rgb8DD=2844,rgb8DE=2845,rgb8DF=2846,rgb8E0=2847,rgb8E1=2848,rgb8E2=2849,rgb8E3=2850,rgb8E4=2851,rgb8E5=2852,rgb8E6=2853,rgb8E7=2854,rgb8E8=2855,rgb8E9=2856,rgb8EA=2857,rgb8EB=2858,rgb8EC=2859,rgb8ED=2860,rgb8EE=2861,rgb8EF=2862,rgb8F0=2863,rgb8F1=2864,rgb8F2=2865,rgb8F3=2866,rgb8F4=2867,rgb8F5=2868,rgb8F6=2869,rgb8F7=2870,rgb8F8=2871,rgb8F9=2872,rgb8FA=2873,rgb8FB=2874,rgb8FC=2875,rgb8FD=2876,rgb8FE=2877,rgb8FF=2878,rgb900=2879,rgb901=2880,rgb902=2881,rgb903=2882,rgb904=2883,rgb905=2884,rgb906=2885,rgb907=2886,rgb908=2887,rgb909=2888,rgb90A=2889,rgb90B=2890,rgb90C=2891,rgb90D=2892,rgb90E=2893,rgb90F=2894,rgb910=2895,rgb911=2896,rgb912=2897,rgb913=2898,rgb914=2899,rgb915=2900,rgb916=2901,rgb917=2902,rgb918=2903,rgb919=2904,rgb91A=2905,rgb91B=2906,rgb91C=2907,rgb91D=2908,rgb91E=2909,rgb91F=2910,rgb920=2911,rgb921=2912,rgb922=2913,rgb923=2914,rgb924=2915,rgb925=2916,rgb926=2917,rgb927=2918,rgb928=2919,rgb929=2920,rgb92A=2921,rgb92B=2922,rgb92C=2923,rgb92D=2924,rgb92E=2925,rgb92F=2926,rgb930=2927,rgb931=2928,rgb932=2929,rgb933=2930,rgb934=2931,rgb935=2932,rgb936=2933,rgb937=2934,rgb938=2935,rgb939=2936,rgb93A=2937,rgb93B=2938,rgb93C=2939,rgb93D=2940,rgb93E=2941,rgb93F=2942,rgb940=2943,rgb941=2944,rgb942=2945,rgb943=2946,rgb944=2947,rgb945=2948,rgb946=2949,rgb947=2950,
rgb948=2951,rgb949=2952,rgb94A=2953,rgb94B=2954,rgb94C=2955,rgb94D=2956,rgb94E=2957,rgb94F=2958,rgb950=2959,rgb951=2960,rgb952=2961,rgb953=2962,rgb954=2963,rgb955=2964,rgb956=2965,rgb957=2966,rgb958=2967,rgb959=2968,rgb95A=2969,rgb95B=2970,rgb95C=2971,rgb95D=2972,rgb95E=2973,rgb95F=2974,rgb960=2975,rgb961=2976,rgb962=2977,rgb963=2978,rgb964=2979,rgb965=2980,rgb966=2981,rgb967=2982,rgb968=2983,rgb969=2984,rgb96A=2985,rgb96B=2986,rgb96C=2987,rgb96D=2988,rgb96E=2989,rgb96F=2990,rgb970=2991,rgb971=2992,rgb972=2993,rgb973=2994,rgb974=2995,rgb975=2996,rgb976=2997,rgb977=2998,rgb978=2999,rgb979=3000,rgb97A=3001,rgb97B=3002,rgb97C=3003,rgb97D=3004,rgb97E=3005,rgb97F=3006,rgb980=3007,rgb981=3008,rgb982=3009,rgb983=3010,rgb984=3011,rgb985=3012,rgb986=3013,rgb987=3014,rgb988=3015,rgb989=3016,rgb98A=3017,rgb98B=3018,rgb98C=3019,rgb98D=3020,rgb98E=3021,rgb98F=3022,rgb990=3023,rgb991=3024,rgb992=3025,rgb993=3026,rgb994=3027,rgb995=3028,rgb996=3029,rgb997=3030,rgb998=3031,rgb999=3032,rgb99A=3033,rgb99B=3034,rgb99C=3035,rgb99D=3036,rgb99E=3037,rgb99F=3038,rgb9A0=3039,rgb9A1=3040,rgb9A2=3041,rgb9A3=3042,rgb9A4=3043,rgb9A5=3044,rgb9A6=3045,rgb9A7=3046,rgb9A8=3047,rgb9A9=3048,rgb9AA=3049,rgb9AB=3050,rgb9AC=3051,rgb9AD=3052,rgb9AE=3053,rgb9AF=3054,rgb9B0=3055,rgb9B1=3056,rgb9B2=3057,rgb9B3=3058,rgb9B4=3059,rgb9B5=3060,rgb9B6=3061,rgb9B7=3062,rgb9B8=3063,rgb9B9=3064,rgb9BA=3065,rgb9BB=3066,rgb9BC=3067,rgb9BD=3068,rgb9BE=3069,rgb9BF=3070,rgb9C0=3071,rgb9C1=3072,rgb9C2=3073,rgb9C3=3074,rgb9C4=3075,rgb9C5=3076,rgb9C6=3077,rgb9C7=3078,rgb9C8=3079,rgb9C9=3080,rgb9CA=3081,rgb9CB=3082,rgb9CC=3083,rgb9CD=3084,rgb9CE=3085,rgb9CF=3086,rgb9D0=3087,rgb9D1=3088,rgb9D2=3089,rgb9D3=3090,rgb9D4=3091,rgb9D5=3092,rgb9D6=3093,rgb9D7=3094,rgb9D8=3095,rgb9D9=3096,rgb9DA=3097,rgb9DB=3098,rgb9DC=3099,rgb9DD=3100,rgb9DE=3101,rgb9DF=3102,rgb9E0=3103,rgb9E1=3104,rgb9E2=3105,rgb9E3=3106,rgb9E4=3107,rgb9E5=3108,rgb9E6=3109,rgb9E7=3110,rgb9E8=3111,rgb9E9=3112,rgb9EA=3113,rgb9EB=3114,rgb9EC=3115,rgb9ED=3116,rgb9EE=3117,rgb9EF=3118,rgb9F0=3119,rgb9F1=3120,rgb9F2=3121,rgb9F3=3122,rgb9F4=3123,rgb9F5=3124,rgb9F6=3125,rgb9F7=3126,rgb9F8=3127,rgb9F9=3128,rgb9FA=3129,rgb9FB=3130,rgb9FC=3131,rgb9FD=3132,rgb9FE=3133,rgb9FF=3134,rgbA00=3135,rgbA01=3136,rgbA02=3137,rgbA03=3138,rgbA04=3139,rgbA05=3140,rgbA06=3141,
rgbA07=3142,rgbA08=3143,rgbA09=3144,rgbA0A=3145,rgbA0B=3146,rgbA0C=3147,rgbA0D=3148,rgbA0E=3149,rgbA0F=3150,rgbA10=3151,rgbA11=3152,rgbA12=3153,rgbA13=3154,rgbA14=3155,rgbA15=3156,rgbA16=3157,rgbA17=3158,rgbA18=3159,rgbA19=3160,rgbA1A=3161,rgbA1B=3162,rgbA1C=3163,rgbA1D=3164,rgbA1E=3165,rgbA1F=3166,rgbA20=3167,rgbA21=3168,rgbA22=3169,rgbA23=3170,rgbA24=3171,rgbA25=3172,rgbA26=3173,rgbA27=3174,rgbA28=3175,rgbA29=3176,rgbA2A=3177,rgbA2B=3178,rgbA2C=3179,rgbA2D=3180,rgbA2E=3181,rgbA2F=3182,rgbA30=3183,rgbA31=3184,rgbA32=3185,rgbA33=3186,rgbA34=3187,rgbA35=3188,rgbA36=3189,rgbA37=3190,rgbA38=3191,rgbA39=3192,rgbA3A=3193,rgbA3B=3194,rgbA3C=3195,rgbA3D=3196,rgbA3E=3197,rgbA3F=3198,rgbA40=3199,rgbA41=3200,rgbA42=3201,rgbA43=3202,rgbA44=3203,rgbA45=3204,rgbA46=3205,rgbA47=3206,rgbA48=3207,rgbA49=3208,rgbA4A=3209,rgbA4B=3210,rgbA4C=3211,rgbA4D=3212,rgbA4E=3213,rgbA4F=3214,rgbA50=3215,rgbA51=3216,rgbA52=3217,rgbA53=3218,rgbA54=3219,rgbA55=3220,rgbA56=3221,rgbA57=3222,rgbA58=3223,rgbA59=3224,rgbA5A=3225,rgbA5B=3226,rgbA5C=3227,rgbA5D=3228,rgbA5E=3229,rgbA5F=3230,rgbA60=3231,rgbA61=3232,rgbA62=3233,rgbA63=3234,rgbA64=3235,rgbA65=3236,rgbA66=3237,rgbA67=3238,rgbA68=3239,rgbA69=3240,rgbA6A=3241,rgbA6B=3242,rgbA6C=3243,rgbA6D=3244,rgbA6E=3245,rgbA6F=3246,rgbA70=3247,rgbA71=3248,rgbA72=3249,rgbA73=3250,rgbA74=3251,rgbA75=3252,rgbA76=3253,rgbA77=3254,rgbA78=3255,rgbA79=3256,rgbA7A=3257,rgbA7B=3258,rgbA7C=3259,rgbA7D=3260,rgbA7E=3261,rgbA7F=3262,rgbA80=3263,rgbA81=3264,rgbA82=3265,rgbA83=3266,rgbA84=3267,rgbA85=3268,rgbA86=3269,rgbA87=3270,rgbA88=3271,rgbA89=3272,rgbA8A=3273,rgbA8B=3274,rgbA8C=3275,rgbA8D=3276,rgbA8E=3277,rgbA8F=3278,rgbA90=3279,rgbA91=3280,rgbA92=3281,rgbA93=3282,rgbA94=3283,rgbA95=3284,rgbA96=3285,rgbA97=3286,rgbA98=3287,rgbA99=3288,rgbA9A=3289,rgbA9B=3290,rgbA9C=3291,rgbA9D=3292,rgbA9E=3293,rgbA9F=3294,rgbAA0=3295,rgbAA1=3296,rgbAA2=3297,rgbAA3=3298,rgbAA4=3299,rgbAA5=3300,rgbAA6=3301,rgbAA7=3302,rgbAA8=3303,rgbAA9=3304,rgbAAA=3305,rgbAAB=3306,rgbAAC=3307,rgbAAD=3308,rgbAAE=3309,rgbAAF=3310,rgbAB0=3311,rgbAB1=3312,rgbAB2=3313,rgbAB3=3314,rgbAB4=3315,rgbAB5=3316,rgbAB6=3317,rgbAB7=3318,rgbAB8=3319,rgbAB9=3320,rgbABA=3321,rgbABB=3322,rgbABC=3323,rgbABD=3324,rgbABE=3325,rgbABF=3326,rgbAC0=3327,rgbAC1=3328,rgbAC2=3329,rgbAC3=3330,rgbAC4=3331,rgbAC5=3332,
rgbAC6=3333,rgbAC7=3334,rgbAC8=3335,rgbAC9=3336,rgbACA=3337,rgbACB=3338,rgbACC=3339,rgbACD=3340,rgbACE=3341,rgbACF=3342,rgbAD0=3343,rgbAD1=3344,rgbAD2=3345,rgbAD3=3346,rgbAD4=3347,rgbAD5=3348,rgbAD6=3349,rgbAD7=3350,rgbAD8=3351,rgbAD9=3352,rgbADA=3353,rgbADB=3354,rgbADC=3355,rgbADD=3356,rgbADE=3357,rgbADF=3358,rgbAE0=3359,rgbAE1=3360,rgbAE2=3361,rgbAE3=3362,rgbAE4=3363,rgbAE5=3364,rgbAE6=3365,rgbAE7=3366,rgbAE8=3367,rgbAE9=3368,rgbAEA=3369,rgbAEB=3370,rgbAEC=3371,rgbAED=3372,rgbAEE=3373,rgbAEF=3374,rgbAF0=3375,rgbAF1=3376,rgbAF2=3377,rgbAF3=3378,rgbAF4=3379,rgbAF5=3380,rgbAF6=3381,rgbAF7=3382,rgbAF8=3383,rgbAF9=3384,rgbAFA=3385,rgbAFB=3386,rgbAFC=3387,rgbAFD=3388,rgbAFE=3389,rgbAFF=3390,rgbB00=3391,rgbB01=3392,rgbB02=3393,rgbB03=3394,rgbB04=3395,rgbB05=3396,rgbB06=3397,rgbB07=3398,rgbB08=3399,rgbB09=3400,rgbB0A=3401,rgbB0B=3402,rgbB0C=3403,rgbB0D=3404,rgbB0E=3405,rgbB0F=3406,rgbB10=3407,rgbB11=3408,rgbB12=3409,rgbB13=3410,rgbB14=3411,rgbB15=3412,rgbB16=3413,rgbB17=3414,rgbB18=3415,rgbB19=3416,rgbB1A=3417,rgbB1B=3418,rgbB1C=3419,rgbB1D=3420,rgbB1E=3421,rgbB1F=3422,rgbB20=3423,rgbB21=3424,rgbB22=3425,rgbB23=3426,rgbB24=3427,rgbB25=3428,rgbB26=3429,rgbB27=3430,rgbB28=3431,rgbB29=3432,rgbB2A=3433,rgbB2B=3434,rgbB2C=3435,rgbB2D=3436,rgbB2E=3437,rgbB2F=3438,rgbB30=3439,rgbB31=3440,rgbB32=3441,rgbB33=3442,rgbB34=3443,rgbB35=3444,rgbB36=3445,rgbB37=3446,rgbB38=3447,rgbB39=3448,rgbB3A=3449,rgbB3B=3450,rgbB3C=3451,rgbB3D=3452,rgbB3E=3453,rgbB3F=3454,rgbB40=3455,rgbB41=3456,rgbB42=3457,rgbB43=3458,rgbB44=3459,rgbB45=3460,rgbB46=3461,rgbB47=3462,rgbB48=3463,rgbB49=3464,rgbB4A=3465,rgbB4B=3466,rgbB4C=3467,rgbB4D=3468,rgbB4E=3469,rgbB4F=3470,rgbB50=3471,rgbB51=3472,rgbB52=3473,rgbB53=3474,rgbB54=3475,rgbB55=3476,rgbB56=3477,rgbB57=3478,rgbB58=3479,rgbB59=3480,rgbB5A=3481,rgbB5B=3482,rgbB5C=3483,rgbB5D=3484,rgbB5E=3485,rgbB5F=3486,rgbB60=3487,rgbB61=3488,
rgbB62=3489,rgbB63=3490,rgbB64=3491,rgbB65=3492,rgbB66=3493,rgbB67=3494,rgbB68=3495,rgbB69=3496,rgbB6A=3497,rgbB6B=3498,rgbB6C=3499,rgbB6D=3500,rgbB6E=3501,rgbB6F=3502,rgbB70=3503,rgbB71=3504,rgbB72=3505,rgbB73=3506,rgbB74=3507,rgbB75=3508,rgbB76=3509,rgbB77=3510,rgbB78=3511,rgbB79=3512,rgbB7A=3513,rgbB7B=3514,rgbB7C=3515,rgbB7D=3516,rgbB7E=3517,rgbB7F=3518,rgbB80=3519,rgbB81=3520,rgbB82=3521,rgbB83=3522,rgbB84=3523,rgbB85=3524,rgbB86=3525,rgbB87=3526,rgbB88=3527,rgbB89=3528,rgbB8A=3529,rgbB8B=3530,rgbB8C=3531,rgbB8D=3532,rgbB8E=3533,rgbB8F=3534,rgbB90=3535,rgbB91=3536,rgbB92=3537,rgbB93=3538,rgbB94=3539,rgbB95=3540,rgbB96=3541,rgbB97=3542,rgbB98=3543,rgbB99=3544,rgbB9A=3545,rgbB9B=3546,rgbB9C=3547,rgbB9D=3548,rgbB9E=3549,rgbB9F=3550,rgbBA0=3551,rgbBA1=3552,rgbBA2=3553,rgbBA3=3554,rgbBA4=3555,rgbBA5=3556,rgbBA6=3557,rgbBA7=3558,rgbBA8=3559,rgbBA9=3560,rgbBAA=3561,rgbBAB=3562,rgbBAC=3563,rgbBAD=3564,rgbBAE=3565,rgbBAF=3566,rgbBB0=3567,rgbBB1=3568,rgbBB2=3569,rgbBB3=3570,rgbBB4=3571,rgbBB5=3572,rgbBB6=3573,rgbBB7=3574,rgbBB8=3575,rgbBB9=3576,rgbBBA=3577,rgbBBB=3578,rgbBBC=3579,rgbBBD=3580,rgbBBE=3581,rgbBBF=3582,rgbBC0=3583,rgbBC1=3584,rgbBC2=3585,rgbBC3=3586,rgbBC4=3587,rgbBC5=3588,rgbBC6=3589,rgbBC7=3590,rgbBC8=3591,rgbBC9=3592,rgbBCA=3593,rgbBCB=3594,rgbBCC=3595,rgbBCD=3596,rgbBCE=3597,rgbBCF=3598,rgbBD0=3599,rgbBD1=3600,rgbBD2=3601,rgbBD3=3602,rgbBD4=3603,rgbBD5=3604,rgbBD6=3605,rgbBD7=3606,rgbBD8=3607,rgbBD9=3608,rgbBDA=3609,rgbBDB=3610,rgbBDC=3611,rgbBDD=3612,rgbBDE=3613,rgbBDF=3614,rgbBE0=3615,rgbBE1=3616,rgbBE2=3617,rgbBE3=3618,rgbBE4=3619,rgbBE5=3620,rgbBE6=3621,rgbBE7=3622,rgbBE8=3623,rgbBE9=3624,rgbBEA=3625,rgbBEB=3626,rgbBEC=3627,rgbBED=3628,rgbBEE=3629,rgbBEF=3630,rgbBF0=3631,rgbBF1=3632,rgbBF2=3633,rgbBF3=3634,rgbBF4=3635,rgbBF5=3636,rgbBF6=3637,rgbBF7=3638,rgbBF8=3639,rgbBF9=3640,rgbBFA=3641,rgbBFB=3642,rgbBFC=3643,rgbBFD=3644,rgbBFE=3645,rgbBFF=3646,rgbC00=3647,rgbC01=3648,rgbC02=3649,rgbC03=3650,rgbC04=3651,rgbC05=3652,rgbC06=3653,rgbC07=3654,rgbC08=3655,rgbC09=3656,rgbC0A=3657,rgbC0B=3658,rgbC0C=3659,rgbC0D=3660,rgbC0E=3661,rgbC0F=3662,rgbC10=3663,rgbC11=3664,rgbC12=3665,rgbC13=3666,rgbC14=3667,rgbC15=3668,rgbC16=3669,rgbC17=3670,rgbC18=3671,rgbC19=3672,rgbC1A=3673,rgbC1B=3674,rgbC1C=3675,rgbC1D=3676,rgbC1E=3677,rgbC1F=3678,rgbC20=3679,
rgbC21=3680,rgbC22=3681,rgbC23=3682,rgbC24=3683,rgbC25=3684,rgbC26=3685,rgbC27=3686,rgbC28=3687,rgbC29=3688,rgbC2A=3689,rgbC2B=3690,rgbC2C=3691,rgbC2D=3692,rgbC2E=3693,rgbC2F=3694,rgbC30=3695,rgbC31=3696,rgbC32=3697,rgbC33=3698,rgbC34=3699,rgbC35=3700,rgbC36=3701,rgbC37=3702,rgbC38=3703,rgbC39=3704,rgbC3A=3705,rgbC3B=3706,rgbC3C=3707,rgbC3D=3708,rgbC3E=3709,rgbC3F=3710,rgbC40=3711,rgbC41=3712,rgbC42=3713,rgbC43=3714,rgbC44=3715,rgbC45=3716,rgbC46=3717,rgbC47=3718,rgbC48=3719,rgbC49=3720,rgbC4A=3721,rgbC4B=3722,rgbC4C=3723,rgbC4D=3724,rgbC4E=3725,rgbC4F=3726,rgbC50=3727,rgbC51=3728,rgbC52=3729,rgbC53=3730,rgbC54=3731,rgbC55=3732,rgbC56=3733,rgbC57=3734,rgbC58=3735,rgbC59=3736,rgbC5A=3737,rgbC5B=3738,rgbC5C=3739,rgbC5D=3740,rgbC5E=3741,rgbC5F=3742,rgbC60=3743,rgbC61=3744,rgbC62=3745,rgbC63=3746,rgbC64=3747,rgbC65=3748,rgbC66=3749,rgbC67=3750,rgbC68=3751,rgbC69=3752,rgbC6A=3753,rgbC6B=3754,rgbC6C=3755,rgbC6D=3756,rgbC6E=3757,rgbC6F=3758,rgbC70=3759,rgbC71=3760,rgbC72=3761,rgbC73=3762,rgbC74=3763,rgbC75=3764,rgbC76=3765,rgbC77=3766,rgbC78=3767,rgbC79=3768,rgbC7A=3769,rgbC7B=3770,rgbC7C=3771,rgbC7D=3772,rgbC7E=3773,rgbC7F=3774,rgbC80=3775,rgbC81=3776,rgbC82=3777,rgbC83=3778,rgbC84=3779,rgbC85=3780,rgbC86=3781,rgbC87=3782,rgbC88=3783,rgbC89=3784,rgbC8A=3785,rgbC8B=3786,rgbC8C=3787,rgbC8D=3788,rgbC8E=3789,rgbC8F=3790,rgbC90=3791,rgbC91=3792,rgbC92=3793,rgbC93=3794,rgbC94=3795,rgbC95=3796,rgbC96=3797,rgbC97=3798,rgbC98=3799,rgbC99=3800,rgbC9A=3801,rgbC9B=3802,rgbC9C=3803,rgbC9D=3804,rgbC9E=3805,rgbC9F=3806,rgbCA0=3807,rgbCA1=3808,rgbCA2=3809,rgbCA3=3810,rgbCA4=3811,rgbCA5=3812,rgbCA6=3813,rgbCA7=3814,rgbCA8=3815,rgbCA9=3816,rgbCAA=3817,rgbCAB=3818,rgbCAC=3819,rgbCAD=3820,rgbCAE=3821,rgbCAF=3822,rgbCB0=3823,rgbCB1=3824,rgbCB2=3825,rgbCB3=3826,rgbCB4=3827,rgbCB5=3828,rgbCB6=3829,rgbCB7=3830,rgbCB8=3831,rgbCB9=3832,rgbCBA=3833,rgbCBB=3834,rgbCBC=3835,rgbCBD=3836,rgbCBE=3837,rgbCBF=3838,rgbCC0=3839,rgbCC1=3840,rgbCC2=3841,rgbCC3=3842,rgbCC4=3843,rgbCC5=3844,rgbCC6=3845,rgbCC7=3846,rgbCC8=3847,rgbCC9=3848,rgbCCA=3849,rgbCCB=3850,rgbCCC=3851,rgbCCD=3852,rgbCCE=3853,rgbCCF=3854,rgbCD0=3855,rgbCD1=3856,rgbCD2=3857,rgbCD3=3858,rgbCD4=3859,rgbCD5=3860,rgbCD6=3861,rgbCD7=3862,rgbCD8=3863,rgbCD9=3864,rgbCDA=3865,rgbCDB=3866,rgbCDC=3867,rgbCDD=3868,rgbCDE=3869,rgbCDF=3870,
rgbCE0=3871,rgbCE1=3872,rgbCE2=3873,rgbCE3=3874,rgbCE4=3875,rgbCE5=3876,rgbCE6=3877,rgbCE7=3878,rgbCE8=3879,rgbCE9=3880,rgbCEA=3881,rgbCEB=3882,rgbCEC=3883,rgbCED=3884,rgbCEE=3885,rgbCEF=3886,rgbCF0=3887,rgbCF1=3888,rgbCF2=3889,rgbCF3=3890,rgbCF4=3891,rgbCF5=3892,rgbCF6=3893,rgbCF7=3894,rgbCF8=3895,rgbCF9=3896,rgbCFA=3897,rgbCFB=3898,rgbCFC=3899,rgbCFD=3900,rgbCFE=3901,rgbCFF=3902,rgbD00=3903,rgbD01=3904,rgbD02=3905,rgbD03=3906,rgbD04=3907,rgbD05=3908,rgbD06=3909,rgbD07=3910,rgbD08=3911,rgbD09=3912,rgbD0A=3913,rgbD0B=3914,rgbD0C=3915,rgbD0D=3916,rgbD0E=3917,rgbD0F=3918,rgbD10=3919,rgbD11=3920,rgbD12=3921,rgbD13=3922,rgbD14=3923,rgbD15=3924,rgbD16=3925,rgbD17=3926,rgbD18=3927,rgbD19=3928,rgbD1A=3929,rgbD1B=3930,rgbD1C=3931,rgbD1D=3932,rgbD1E=3933,rgbD1F=3934,rgbD20=3935,rgbD21=3936,rgbD22=3937,rgbD23=3938,rgbD24=3939,rgbD25=3940,rgbD26=3941,rgbD27=3942,rgbD28=3943,rgbD29=3944,rgbD2A=3945,rgbD2B=3946,rgbD2C=3947,rgbD2D=3948,rgbD2E=3949,rgbD2F=3950,rgbD30=3951,rgbD31=3952,rgbD32=3953,rgbD33=3954,rgbD34=3955,rgbD35=3956,rgbD36=3957,rgbD37=3958,rgbD38=3959,rgbD39=3960,rgbD3A=3961,rgbD3B=3962,rgbD3C=3963,rgbD3D=3964,rgbD3E=3965,rgbD3F=3966,rgbD40=3967,rgbD41=3968,rgbD42=3969,rgbD43=3970,rgbD44=3971,rgbD45=3972,rgbD46=3973,rgbD47=3974,rgbD48=3975,rgbD49=3976,rgbD4A=3977,rgbD4B=3978,rgbD4C=3979,rgbD4D=3980,rgbD4E=3981,rgbD4F=3982,rgbD50=3983,rgbD51=3984,rgbD52=3985,rgbD53=3986,rgbD54=3987,rgbD55=3988,rgbD56=3989,rgbD57=3990,rgbD58=3991,rgbD59=3992,rgbD5A=3993,rgbD5B=3994,rgbD5C=3995,rgbD5D=3996,rgbD5E=3997,rgbD5F=3998,rgbD60=3999,rgbD61=4000,rgbD62=4001,rgbD63=4002,rgbD64=4003,rgbD65=4004,rgbD66=4005,rgbD67=4006,rgbD68=4007,rgbD69=4008,rgbD6A=4009,rgbD6B=4010,rgbD6C=4011,rgbD6D=4012,rgbD6E=4013,rgbD6F=4014,rgbD70=4015,rgbD71=4016,rgbD72=4017,rgbD73=4018,rgbD74=4019,rgbD75=4020,rgbD76=4021,rgbD77=4022,rgbD78=4023,rgbD79=4024,rgbD7A=4025,rgbD7B=4026,
rgbD7C=4027,rgbD7D=4028,rgbD7E=4029,rgbD7F=4030,rgbD80=4031,rgbD81=4032,rgbD82=4033,rgbD83=4034,rgbD84=4035,rgbD85=4036,rgbD86=4037,rgbD87=4038,rgbD88=4039,rgbD89=4040,rgbD8A=4041,rgbD8B=4042,rgbD8C=4043,rgbD8D=4044,rgbD8E=4045,rgbD8F=4046,rgbD90=4047,rgbD91=4048,rgbD92=4049,rgbD93=4050,rgbD94=4051,rgbD95=4052,rgbD96=4053,rgbD97=4054,rgbD98=4055,rgbD99=4056,rgbD9A=4057,rgbD9B=4058,rgbD9C=4059,rgbD9D=4060,rgbD9E=4061,rgbD9F=4062,rgbDA0=4063,rgbDA1=4064,rgbDA2=4065,rgbDA3=4066,rgbDA4=4067,rgbDA5=4068,rgbDA6=4069,rgbDA7=4070,rgbDA8=4071,rgbDA9=4072,rgbDAA=4073,rgbDAB=4074,rgbDAC=4075,rgbDAD=4076,rgbDAE=4077,rgbDAF=4078,rgbDB0=4079,rgbDB1=4080,rgbDB2=4081,rgbDB3=4082,rgbDB4=4083,rgbDB5=4084,rgbDB6=4085,rgbDB7=4086,rgbDB8=4087,rgbDB9=4088,rgbDBA=4089,rgbDBB=4090,rgbDBC=4091,rgbDBD=4092,rgbDBE=4093,rgbDBF=4094,rgbDC0=4095,rgbDC1=4096,rgbDC2=4097,rgbDC3=4098,rgbDC4=4099,rgbDC5=4100,rgbDC6=4101,rgbDC7=4102,rgbDC8=4103,rgbDC9=4104,rgbDCA=4105,rgbDCB=4106,rgbDCC=4107,rgbDCD=4108,rgbDCE=4109,rgbDCF=4110,rgbDD0=4111,rgbDD1=4112,rgbDD2=4113,rgbDD3=4114,rgbDD4=4115,rgbDD5=4116,rgbDD6=4117,rgbDD7=4118,rgbDD8=4119,rgbDD9=4120,rgbDDA=4121,rgbDDB=4122,rgbDDC=4123,rgbDDD=4124,rgbDDE=4125,rgbDDF=4126,rgbDE0=4127,rgbDE1=4128,rgbDE2=4129,rgbDE3=4130,rgbDE4=4131,rgbDE5=4132,rgbDE6=4133,rgbDE7=4134,rgbDE8=4135,rgbDE9=4136,rgbDEA=4137,rgbDEB=4138,rgbDEC=4139,rgbDED=4140,rgbDEE=4141,rgbDEF=4142,rgbDF0=4143,rgbDF1=4144,rgbDF2=4145,rgbDF3=4146,rgbDF4=4147,rgbDF5=4148,rgbDF6=4149,rgbDF7=4150,rgbDF8=4151,rgbDF9=4152,rgbDFA=4153,rgbDFB=4154,rgbDFC=4155,rgbDFD=4156,rgbDFE=4157,rgbDFF=4158,rgbE00=4159,rgbE01=4160,rgbE02=4161,rgbE03=4162,rgbE04=4163,rgbE05=4164,rgbE06=4165,rgbE07=4166,rgbE08=4167,rgbE09=4168,rgbE0A=4169,rgbE0B=4170,rgbE0C=4171,rgbE0D=4172,rgbE0E=4173,rgbE0F=4174,rgbE10=4175,rgbE11=4176,rgbE12=4177,rgbE13=4178,rgbE14=4179,rgbE15=4180,rgbE16=4181,rgbE17=4182,rgbE18=4183,rgbE19=4184,rgbE1A=4185,rgbE1B=4186,rgbE1C=4187,rgbE1D=4188,rgbE1E=4189,rgbE1F=4190,rgbE20=4191,rgbE21=4192,rgbE22=4193,rgbE23=4194,rgbE24=4195,rgbE25=4196,rgbE26=4197,rgbE27=4198,rgbE28=4199,rgbE29=4200,rgbE2A=4201,rgbE2B=4202,rgbE2C=4203,rgbE2D=4204,rgbE2E=4205,rgbE2F=4206,rgbE30=4207,rgbE31=4208,rgbE32=4209,rgbE33=4210,rgbE34=4211,rgbE35=4212,rgbE36=4213,rgbE37=4214,rgbE38=4215,rgbE39=4216,rgbE3A=4217,
rgbE3B=4218,rgbE3C=4219,rgbE3D=4220,rgbE3E=4221,rgbE3F=4222,rgbE40=4223,rgbE41=4224,rgbE42=4225,rgbE43=4226,rgbE44=4227,rgbE45=4228,rgbE46=4229,rgbE47=4230,rgbE48=4231,rgbE49=4232,rgbE4A=4233,rgbE4B=4234,rgbE4C=4235,rgbE4D=4236,rgbE4E=4237,rgbE4F=4238,rgbE50=4239,rgbE51=4240,rgbE52=4241,rgbE53=4242,rgbE54=4243,rgbE55=4244,rgbE56=4245,rgbE57=4246,rgbE58=4247,rgbE59=4248,rgbE5A=4249,rgbE5B=4250,rgbE5C=4251,rgbE5D=4252,rgbE5E=4253,rgbE5F=4254,rgbE60=4255,rgbE61=4256,rgbE62=4257,rgbE63=4258,rgbE64=4259,rgbE65=4260,rgbE66=4261,rgbE67=4262,rgbE68=4263,rgbE69=4264,rgbE6A=4265,rgbE6B=4266,rgbE6C=4267,rgbE6D=4268,rgbE6E=4269,rgbE6F=4270,rgbE70=4271,rgbE71=4272,rgbE72=4273,rgbE73=4274,rgbE74=4275,rgbE75=4276,rgbE76=4277,rgbE77=4278,rgbE78=4279,rgbE79=4280,rgbE7A=4281,rgbE7B=4282,rgbE7C=4283,rgbE7D=4284,rgbE7E=4285,rgbE7F=4286,rgbE80=4287,rgbE81=4288,rgbE82=4289,rgbE83=4290,rgbE84=4291,rgbE85=4292,rgbE86=4293,rgbE87=4294,rgbE88=4295,rgbE89=4296,rgbE8A=4297,rgbE8B=4298,rgbE8C=4299,rgbE8D=4300,rgbE8E=4301,rgbE8F=4302,rgbE90=4303,rgbE91=4304,rgbE92=4305,rgbE93=4306,rgbE94=4307,rgbE95=4308,rgbE96=4309,rgbE97=4310,rgbE98=4311,rgbE99=4312,rgbE9A=4313,rgbE9B=4314,rgbE9C=4315,rgbE9D=4316,rgbE9E=4317,rgbE9F=4318,rgbEA0=4319,rgbEA1=4320,rgbEA2=4321,rgbEA3=4322,rgbEA4=4323,rgbEA5=4324,rgbEA6=4325,rgbEA7=4326,rgbEA8=4327,rgbEA9=4328,rgbEAA=4329,rgbEAB=4330,rgbEAC=4331,rgbEAD=4332,rgbEAE=4333,rgbEAF=4334,rgbEB0=4335,rgbEB1=4336,rgbEB2=4337,rgbEB3=4338,rgbEB4=4339,rgbEB5=4340,rgbEB6=4341,rgbEB7=4342,rgbEB8=4343,rgbEB9=4344,rgbEBA=4345,rgbEBB=4346,rgbEBC=4347,rgbEBD=4348,rgbEBE=4349,rgbEBF=4350,rgbEC0=4351,rgbEC1=4352,rgbEC2=4353,rgbEC3=4354,rgbEC4=4355,rgbEC5=4356,rgbEC6=4357,rgbEC7=4358,rgbEC8=4359,rgbEC9=4360,rgbECA=4361,rgbECB=4362,rgbECC=4363,rgbECD=4364,rgbECE=4365,rgbECF=4366,rgbED0=4367,rgbED1=4368,rgbED2=4369,rgbED3=4370,rgbED4=4371,rgbED5=4372,rgbED6=4373,rgbED7=4374,rgbED8=4375,rgbED9=4376,rgbEDA=4377,rgbEDB=4378,rgbEDC=4379,rgbEDD=4380,rgbEDE=4381,rgbEDF=4382,rgbEE0=4383,rgbEE1=4384,rgbEE2=4385,rgbEE3=4386,rgbEE4=4387,rgbEE5=4388,rgbEE6=4389,rgbEE7=4390,rgbEE8=4391,rgbEE9=4392,rgbEEA=4393,rgbEEB=4394,rgbEEC=4395,rgbEED=4396,rgbEEE=4397,rgbEEF=4398,rgbEF0=4399,rgbEF1=4400,rgbEF2=4401,rgbEF3=4402,rgbEF4=4403,rgbEF5=4404,rgbEF6=4405,rgbEF7=4406,rgbEF8=4407,rgbEF9=4408,
rgbEFA=4409,rgbEFB=4410,rgbEFC=4411,rgbEFD=4412,rgbEFE=4413,rgbEFF=4414,rgbF00=4415,rgbF01=4416,rgbF02=4417,rgbF03=4418,rgbF04=4419,rgbF05=4420,rgbF06=4421,rgbF07=4422,rgbF08=4423,rgbF09=4424,rgbF0A=4425,rgbF0B=4426,rgbF0C=4427,rgbF0D=4428,rgbF0E=4429,rgbF0F=4430,rgbF10=4431,rgbF11=4432,rgbF12=4433,rgbF13=4434,rgbF14=4435,rgbF15=4436,rgbF16=4437,rgbF17=4438,rgbF18=4439,rgbF19=4440,rgbF1A=4441,rgbF1B=4442,rgbF1C=4443,rgbF1D=4444,rgbF1E=4445,rgbF1F=4446,rgbF20=4447,rgbF21=4448,rgbF22=4449,rgbF23=4450,rgbF24=4451,rgbF25=4452,rgbF26=4453,rgbF27=4454,rgbF28=4455,rgbF29=4456,rgbF2A=4457,rgbF2B=4458,rgbF2C=4459,rgbF2D=4460,rgbF2E=4461,rgbF2F=4462,rgbF30=4463,rgbF31=4464,rgbF32=4465,rgbF33=4466,rgbF34=4467,rgbF35=4468,rgbF36=4469,rgbF37=4470,rgbF38=4471,rgbF39=4472,rgbF3A=4473,rgbF3B=4474,rgbF3C=4475,rgbF3D=4476,rgbF3E=4477,rgbF3F=4478,rgbF40=4479,rgbF41=4480,rgbF42=4481,rgbF43=4482,rgbF44=4483,rgbF45=4484,rgbF46=4485,rgbF47=4486,rgbF48=4487,rgbF49=4488,rgbF4A=4489,rgbF4B=4490,rgbF4C=4491,rgbF4D=4492,rgbF4E=4493,rgbF4F=4494,rgbF50=4495,rgbF51=4496,rgbF52=4497,rgbF53=4498,rgbF54=4499,rgbF55=4500,rgbF56=4501,rgbF57=4502,rgbF58=4503,rgbF59=4504,rgbF5A=4505,rgbF5B=4506,rgbF5C=4507,rgbF5D=4508,rgbF5E=4509,rgbF5F=4510,rgbF60=4511,rgbF61=4512,rgbF62=4513,rgbF63=4514,rgbF64=4515,rgbF65=4516,rgbF66=4517,rgbF67=4518,rgbF68=4519,rgbF69=4520,rgbF6A=4521,rgbF6B=4522,rgbF6C=4523,rgbF6D=4524,rgbF6E=4525,rgbF6F=4526,rgbF70=4527,rgbF71=4528,rgbF72=4529,rgbF73=4530,rgbF74=4531,rgbF75=4532,rgbF76=4533,rgbF77=4534,rgbF78=4535,rgbF79=4536,rgbF7A=4537,rgbF7B=4538,rgbF7C=4539,rgbF7D=4540,rgbF7E=4541,rgbF7F=4542,rgbF80=4543,rgbF81=4544,rgbF82=4545,rgbF83=4546,rgbF84=4547,rgbF85=4548,rgbF86=4549,rgbF87=4550,rgbF88=4551,rgbF89=4552,rgbF8A=4553,rgbF8B=4554,rgbF8C=4555,rgbF8D=4556,rgbF8E=4557,rgbF8F=4558,rgbF90=4559,rgbF91=4560,rgbF92=4561,rgbF93=4562,rgbF94=4563,rgbF95=4564,rgbF96=4565,rgbF97=4566,rgbF98=4567,rgbF99=4568,rgbF9A=4569,rgbF9B=4570,rgbF9C=4571,rgbF9D=4572,rgbF9E=4573,rgbF9F=4574,rgbFA0=4575,rgbFA1=4576,rgbFA2=4577,rgbFA3=4578,rgbFA4=4579,rgbFA5=4580,rgbFA6=4581,rgbFA7=4582,rgbFA8=4583,rgbFA9=4584,rgbFAA=4585,rgbFAB=4586,rgbFAC=4587,rgbFAD=4588,rgbFAE=4589,rgbFAF=4590,rgbFB0=4591,rgbFB1=4592,rgbFB2=4593,rgbFB3=4594,rgbFB4=4595,rgbFB5=4596,rgbFB6=4597,rgbFB7=4598,rgbFB8=4599,
rgbFB9=4600,rgbFBA=4601,rgbFBB=4602,rgbFBC=4603,rgbFBD=4604,rgbFBE=4605,rgbFBF=4606,rgbFC0=4607,rgbFC1=4608,rgbFC2=4609,rgbFC3=4610,rgbFC4=4611,rgbFC5=4612,rgbFC6=4613,rgbFC7=4614,rgbFC8=4615,rgbFC9=4616,rgbFCA=4617,rgbFCB=4618,rgbFCC=4619,rgbFCD=4620,rgbFCE=4621,rgbFCF=4622,rgbFD0=4623,rgbFD1=4624,rgbFD2=4625,rgbFD3=4626,rgbFD4=4627,rgbFD5=4628,rgbFD6=4629,rgbFD7=4630,rgbFD8=4631,rgbFD9=4632,rgbFDA=4633,rgbFDB=4634,rgbFDC=4635,rgbFDD=4636,rgbFDE=4637,rgbFDF=4638,rgbFE0=4639,rgbFE1=4640,rgbFE2=4641,rgbFE3=4642,rgbFE4=4643,rgbFE5=4644,rgbFE6=4645,rgbFE7=4646,rgbFE8=4647,rgbFE9=4648,rgbFEA=4649,rgbFEB=4650,rgbFEC=4651,rgbFED=4652,rgbFEE=4653,rgbFEF=4654,rgbFF0=4655,rgbFF1=4656,rgbFF2=4657,rgbFF3=4658,rgbFF4=4659,rgbFF5=4660,rgbFF6=4661,rgbFF7=4662,rgbFF8=4663,rgbFF9=4664,rgbFFA=4665,rgbFFB=4666,rgbFFC=4667,rgbFFD=4668,rgbFFE=4669,rgbFFF=4670,
rgb0001=4671,rgb0021=4672,rgb0041=4673,rgb0061=4674,rgb0081=4675,rgb00A1=4676,rgb00C1=4677,rgb00E1=4678,rgb0201=4679,rgb0221=4680,rgb0241=4681,rgb0261=4682,rgb0281=4683,rgb02A1=4684,rgb02C1=4685,rgb02E1=4686,rgb0401=4687,rgb0421=4688,rgb0441=4689,rgb0461=4690,rgb0481=4691,rgb04A1=4692,rgb04C1=4693,rgb04E1=4694,rgb0601=4695,rgb0621=4696,rgb0641=4697,rgb0661=4698,rgb0681=4699,rgb06A1=4700,rgb06C1=4701,rgb06E1=4702,rgb0801=4703,rgb0821=4704,rgb0841=4705,rgb0861=4706,rgb0881=4707,rgb08A1=4708,rgb08C1=4709,rgb08E1=4710,rgb0A01=4711,rgb0A21=4712,rgb0A41=4713,rgb0A61=4714,rgb0A81=4715,rgb0AA1=4716,rgb0AC1=4717,rgb0AE1=4718,rgb0C01=4719,rgb0C21=4720,rgb0C41=4721,rgb0C61=4722,rgb0C81=4723,rgb0CA1=4724,rgb0CC1=4725,rgb0CE1=4726,rgb0E01=4727,rgb0E21=4728,rgb0E41=4729,rgb0E61=4730,rgb0E81=4731,rgb0EA1=4732,rgb0EC1=4733,rgb0EE1=4734,rgb2001=4735,rgb2021=4736,rgb2041=4737,rgb2061=4738,rgb2081=4739,rgb20A1=4740,rgb20C1=4741,rgb20E1=4742,rgb2201=4743,rgb2221=4744,rgb2241=4745,rgb2261=4746,rgb2281=4747,rgb22A1=4748,rgb22C1=4749,
rgb22E1=4750,rgb2401=4751,rgb2421=4752,rgb2441=4753,rgb2461=4754,rgb2481=4755,rgb24A1=4756,rgb24C1=4757,rgb24E1=4758,rgb2601=4759,rgb2621=4760,rgb2641=4761,rgb2661=4762,rgb2681=4763,rgb26A1=4764,rgb26C1=4765,rgb26E1=4766,rgb2801=4767,rgb2821=4768,rgb2841=4769,rgb2861=4770,rgb2881=4771,rgb28A1=4772,rgb28C1=4773,rgb28E1=4774,rgb2A01=4775,rgb2A21=4776,rgb2A41=4777,rgb2A61=4778,rgb2A81=4779,rgb2AA1=4780,rgb2AC1=4781,rgb2AE1=4782,rgb2C01=4783,rgb2C21=4784,rgb2C41=4785,rgb2C61=4786,rgb2C81=4787,rgb2CA1=4788,rgb2CC1=4789,rgb2CE1=4790,rgb2E01=4791,rgb2E21=4792,rgb2E41=4793,rgb2E61=4794,rgb2E81=4795,rgb2EA1=4796,rgb2EC1=4797,rgb2EE1=4798,rgb4001=4799,rgb4021=4800,rgb4041=4801,rgb4061=4802,rgb4081=4803,rgb40A1=4804,rgb40C1=4805,rgb40E1=4806,rgb4201=4807,rgb4221=4808,rgb4241=4809,rgb4261=4810,rgb4281=4811,rgb42A1=4812,rgb42C1=4813,rgb42E1=4814,rgb4401=4815,rgb4421=4816,rgb4441=4817,rgb4461=4818,rgb4481=4819,rgb44A1=4820,rgb44C1=4821,rgb44E1=4822,rgb4601=4823,rgb4621=4824,rgb4641=4825,rgb4661=4826,rgb4681=4827,rgb46A1=4828,rgb46C1=4829,rgb46E1=4830,rgb4801=4831,rgb4821=4832,rgb4841=4833,rgb4861=4834,rgb4881=4835,rgb48A1=4836,rgb48C1=4837,rgb48E1=4838,rgb4A01=4839,rgb4A21=4840,rgb4A41=4841,rgb4A61=4842,rgb4A81=4843,rgb4AA1=4844,rgb4AC1=4845,rgb4AE1=4846,rgb4C01=4847,rgb4C21=4848,rgb4C41=4849,rgb4C61=4850,rgb4C81=4851,rgb4CA1=4852,rgb4CC1=4853,rgb4CE1=4854,rgb4E01=4855,rgb4E21=4856,rgb4E41=4857,rgb4E61=4858,rgb4E81=4859,rgb4EA1=4860,rgb4EC1=4861,rgb4EE1=4862,rgb6001=4863,rgb6021=4864,rgb6041=4865,rgb6061=4866,rgb6081=4867,rgb60A1=4868,rgb60C1=4869,rgb60E1=4870,rgb6201=4871,rgb6221=4872,rgb6241=4873,rgb6261=4874,rgb6281=4875,rgb62A1=4876,rgb62C1=4877,rgb62E1=4878,rgb6401=4879,rgb6421=4880,rgb6441=4881,rgb6461=4882,rgb6481=4883,rgb64A1=4884,rgb64C1=4885,rgb64E1=4886,rgb6601=4887,rgb6621=4888,rgb6641=4889,rgb6661=4890,rgb6681=4891,rgb66A1=4892,rgb66C1=4893,rgb66E1=4894,rgb6801=4895,rgb6821=4896,rgb6841=4897,rgb6861=4898,rgb6881=4899,rgb68A1=4900,rgb68C1=4901,rgb68E1=4902,rgb6A01=4903,rgb6A21=4904,rgb6A41=4905,rgb6A61=4906,rgb6A81=4907,rgb6AA1=4908,rgb6AC1=4909,rgb6AE1=4910,rgb6C01=4911,rgb6C21=4912,rgb6C41=4913,rgb6C61=4914,rgb6C81=4915,rgb6CA1=4916,rgb6CC1=4917,rgb6CE1=4918,rgb6E01=4919,rgb6E21=4920,rgb6E41=4921,rgb6E61=4922,rgb6E81=4923,rgb6EA1=4924,rgb6EC1=4925,
rgb6EE1=4926,rgb8001=4927,rgb8021=4928,rgb8041=4929,rgb8061=4930,rgb8081=4931,rgb80A1=4932,rgb80C1=4933,rgb80E1=4934,rgb8201=4935,rgb8221=4936,rgb8241=4937,rgb8261=4938,rgb8281=4939,rgb82A1=4940,rgb82C1=4941,rgb82E1=4942,rgb8401=4943,rgb8421=4944,rgb8441=4945,rgb8461=4946,rgb8481=4947,rgb84A1=4948,rgb84C1=4949,rgb84E1=4950,rgb8601=4951,rgb8621=4952,rgb8641=4953,rgb8661=4954,rgb8681=4955,rgb86A1=4956,rgb86C1=4957,rgb86E1=4958,rgb8801=4959,rgb8821=4960,rgb8841=4961,rgb8861=4962,rgb8881=4963,rgb88A1=4964,rgb88C1=4965,rgb88E1=4966,rgb8A01=4967,rgb8A21=4968,rgb8A41=4969,rgb8A61=4970,rgb8A81=4971,rgb8AA1=4972,rgb8AC1=4973,rgb8AE1=4974,rgb8C01=4975,rgb8C21=4976,rgb8C41=4977,rgb8C61=4978,rgb8C81=4979,rgb8CA1=4980,rgb8CC1=4981,rgb8CE1=4982,rgb8E01=4983,rgb8E21=4984,rgb8E41=4985,rgb8E61=4986,rgb8E81=4987,rgb8EA1=4988,rgb8EC1=4989,rgb8EE1=4990,rgbA001=4991,rgbA021=4992,rgbA041=4993,rgbA061=4994,rgbA081=4995,rgbA0A1=4996,rgbA0C1=4997,rgbA0E1=4998,rgbA201=4999,rgbA221=5000,rgbA241=5001,rgbA261=5002,rgbA281=5003,rgbA2A1=5004,rgbA2C1=5005,rgbA2E1=5006,rgbA401=5007,rgbA421=5008,rgbA441=5009,rgbA461=5010,rgbA481=5011,rgbA4A1=5012,rgbA4C1=5013,rgbA4E1=5014,rgbA601=5015,rgbA621=5016,rgbA641=5017,rgbA661=5018,rgbA681=5019,rgbA6A1=5020,rgbA6C1=5021,rgbA6E1=5022,rgbA801=5023,rgbA821=5024,rgbA841=5025,rgbA861=5026,rgbA881=5027,rgbA8A1=5028,rgbA8C1=5029,rgbA8E1=5030,rgbAA01=5031,rgbAA21=5032,rgbAA41=5033,rgbAA61=5034,rgbAA81=5035,rgbAAA1=5036,rgbAAC1=5037,rgbAAE1=5038,rgbAC01=5039,rgbAC21=5040,rgbAC41=5041,rgbAC61=5042,rgbAC81=5043,rgbACA1=5044,rgbACC1=5045,rgbACE1=5046,rgbAE01=5047,rgbAE21=5048,rgbAE41=5049,rgbAE61=5050,rgbAE81=5051,rgbAEA1=5052,rgbAEC1=5053,rgbAEE1=5054,rgbC001=5055,rgbC021=5056,rgbC041=5057,rgbC061=5058,rgbC081=5059,rgbC0A1=5060,rgbC0C1=5061,rgbC0E1=5062,rgbC201=5063,rgbC221=5064,rgbC241=5065,rgbC261=5066,rgbC281=5067,rgbC2A1=5068,rgbC2C1=5069,rgbC2E1=5070,rgbC401=5071,rgbC421=5072,rgbC441=5073,rgbC461=5074,rgbC481=5075,rgbC4A1=5076,rgbC4C1=5077,rgbC4E1=5078,rgbC601=5079,rgbC621=5080,rgbC641=5081,rgbC661=5082,rgbC681=5083,rgbC6A1=5084,rgbC6C1=5085,rgbC6E1=5086,rgbC801=5087,rgbC821=5088,rgbC841=5089,rgbC861=5090,rgbC881=5091,rgbC8A1=5092,rgbC8C1=5093,rgbC8E1=5094,rgbCA01=5095,rgbCA21=5096,rgbCA41=5097,rgbCA61=5098,rgbCA81=5099,rgbCAA1=5100,rgbCAC1=5101,
rgbCAE1=5102,rgbCC01=5103,rgbCC21=5104,rgbCC41=5105,rgbCC61=5106,rgbCC81=5107,rgbCCA1=5108,rgbCCC1=5109,rgbCCE1=5110,rgbCE01=5111,rgbCE21=5112,rgbCE41=5113,rgbCE61=5114,rgbCE81=5115,rgbCEA1=5116,rgbCEC1=5117,rgbCEE1=5118,rgbE001=5119,rgbE021=5120,rgbE041=5121,rgbE061=5122,rgbE081=5123,rgbE0A1=5124,rgbE0C1=5125,rgbE0E1=5126,rgbE201=5127,rgbE221=5128,rgbE241=5129,rgbE261=5130,rgbE281=5131,rgbE2A1=5132,rgbE2C1=5133,rgbE2E1=5134,rgbE401=5135,rgbE421=5136,rgbE441=5137,rgbE461=5138,rgbE481=5139,rgbE4A1=5140,rgbE4C1=5141,rgbE4E1=5142,rgbE601=5143,rgbE621=5144,rgbE641=5145,rgbE661=5146,rgbE681=5147,rgbE6A1=5148,rgbE6C1=5149,rgbE6E1=5150,rgbE801=5151,rgbE821=5152,rgbE841=5153,rgbE861=5154,rgbE881=5155,rgbE8A1=5156,rgbE8C1=5157,rgbE8E1=5158,rgbEA01=5159,rgbEA21=5160,rgbEA41=5161,rgbEA61=5162,rgbEA81=5163,rgbEAA1=5164,rgbEAC1=5165,rgbEAE1=5166,rgbEC01=5167,rgbEC21=5168,rgbEC41=5169,rgbEC61=5170,rgbEC81=5171,rgbECA1=5172,rgbECC1=5173,rgbECE1=5174,rgbEE01=5175,rgbEE21=5176,rgbEE41=5177,rgbEE61=5178,rgbEE81=5179,rgbEEA1=5180,rgbEEC1=5181,rgbEEE1=5182,rgb0002=5183,rgb0022=5184,rgb0042=5185,rgb0062=5186,rgb0082=5187,rgb00A2=5188,rgb00C2=5189,rgb00E2=5190,rgb0202=5191,rgb0222=5192,rgb0242=5193,rgb0262=5194,rgb0282=5195,rgb02A2=5196,rgb02C2=5197,rgb02E2=5198,rgb0402=5199,rgb0422=5200,rgb0442=5201,rgb0462=5202,rgb0482=5203,rgb04A2=5204,rgb04C2=5205,rgb04E2=5206,rgb0602=5207,rgb0622=5208,rgb0642=5209,rgb0662=5210,rgb0682=5211,rgb06A2=5212,rgb06C2=5213,rgb06E2=5214,rgb0802=5215,rgb0822=5216,rgb0842=5217,rgb0862=5218,rgb0882=5219,rgb08A2=5220,rgb08C2=5221,rgb08E2=5222,rgb0A02=5223,rgb0A22=5224,rgb0A42=5225,rgb0A62=5226,rgb0A82=5227,rgb0AA2=5228,rgb0AC2=5229,rgb0AE2=5230,rgb0C02=5231,rgb0C22=5232,rgb0C42=5233,rgb0C62=5234,rgb0C82=5235,rgb0CA2=5236,rgb0CC2=5237,rgb0CE2=5238,rgb0E02=5239,rgb0E22=5240,rgb0E42=5241,rgb0E62=5242,rgb0E82=5243,rgb0EA2=5244,rgb0EC2=5245,rgb0EE2=5246,rgb2002=5247,rgb2022=5248,rgb2042=5249,
rgb2062=5250,rgb2082=5251,rgb20A2=5252,rgb20C2=5253,rgb20E2=5254,rgb2202=5255,rgb2222=5256,rgb2242=5257,rgb2262=5258,rgb2282=5259,rgb22A2=5260,rgb22C2=5261,rgb22E2=5262,rgb2402=5263,rgb2422=5264,rgb2442=5265,rgb2462=5266,rgb2482=5267,rgb24A2=5268,rgb24C2=5269,rgb24E2=5270,rgb2602=5271,rgb2622=5272,rgb2642=5273,rgb2662=5274,rgb2682=5275,rgb26A2=5276,rgb26C2=5277,rgb26E2=5278,rgb2802=5279,rgb2822=5280,rgb2842=5281,rgb2862=5282,rgb2882=5283,rgb28A2=5284,rgb28C2=5285,rgb28E2=5286,rgb2A02=5287,rgb2A22=5288,rgb2A42=5289,rgb2A62=5290,rgb2A82=5291,rgb2AA2=5292,rgb2AC2=5293,rgb2AE2=5294,rgb2C02=5295,rgb2C22=5296,rgb2C42=5297,rgb2C62=5298,rgb2C82=5299,rgb2CA2=5300,rgb2CC2=5301,rgb2CE2=5302,rgb2E02=5303,rgb2E22=5304,rgb2E42=5305,rgb2E62=5306,rgb2E82=5307,rgb2EA2=5308,rgb2EC2=5309,rgb2EE2=5310,rgb4002=5311,rgb4022=5312,rgb4042=5313,rgb4062=5314,rgb4082=5315,rgb40A2=5316,rgb40C2=5317,rgb40E2=5318,rgb4202=5319,rgb4222=5320,rgb4242=5321,rgb4262=5322,rgb4282=5323,rgb42A2=5324,rgb42C2=5325,rgb42E2=5326,rgb4402=5327,rgb4422=5328,rgb4442=5329,rgb4462=5330,rgb4482=5331,rgb44A2=5332,rgb44C2=5333,rgb44E2=5334,rgb4602=5335,rgb4622=5336,rgb4642=5337,rgb4662=5338,rgb4682=5339,rgb46A2=5340,rgb46C2=5341,rgb46E2=5342,rgb4802=5343,rgb4822=5344,rgb4842=5345,rgb4862=5346,rgb4882=5347,rgb48A2=5348,rgb48C2=5349,rgb48E2=5350,rgb4A02=5351,rgb4A22=5352,rgb4A42=5353,rgb4A62=5354,rgb4A82=5355,rgb4AA2=5356,rgb4AC2=5357,rgb4AE2=5358,rgb4C02=5359,rgb4C22=5360,rgb4C42=5361,rgb4C62=5362,rgb4C82=5363,rgb4CA2=5364,rgb4CC2=5365,rgb4CE2=5366,rgb4E02=5367,rgb4E22=5368,rgb4E42=5369,rgb4E62=5370,rgb4E82=5371,rgb4EA2=5372,rgb4EC2=5373,rgb4EE2=5374,rgb6002=5375,rgb6022=5376,rgb6042=5377,rgb6062=5378,rgb6082=5379,rgb60A2=5380,rgb60C2=5381,rgb60E2=5382,rgb6202=5383,rgb6222=5384,rgb6242=5385,rgb6262=5386,rgb6282=5387,rgb62A2=5388,rgb62C2=5389,rgb62E2=5390,rgb6402=5391,rgb6422=5392,rgb6442=5393,rgb6462=5394,rgb6482=5395,rgb64A2=5396,rgb64C2=5397,rgb64E2=5398,rgb6602=5399,rgb6622=5400,rgb6642=5401,rgb6662=5402,rgb6682=5403,rgb66A2=5404,rgb66C2=5405,rgb66E2=5406,rgb6802=5407,rgb6822=5408,rgb6842=5409,rgb6862=5410,rgb6882=5411,rgb68A2=5412,rgb68C2=5413,rgb68E2=5414,rgb6A02=5415,rgb6A22=5416,rgb6A42=5417,rgb6A62=5418,rgb6A82=5419,rgb6AA2=5420,rgb6AC2=5421,rgb6AE2=5422,rgb6C02=5423,rgb6C22=5424,rgb6C42=5425,
rgb6C62=5426,rgb6C82=5427,rgb6CA2=5428,rgb6CC2=5429,rgb6CE2=5430,rgb6E02=5431,rgb6E22=5432,rgb6E42=5433,rgb6E62=5434,rgb6E82=5435,rgb6EA2=5436,rgb6EC2=5437,rgb6EE2=5438,rgb8002=5439,rgb8022=5440,rgb8042=5441,rgb8062=5442,rgb8082=5443,rgb80A2=5444,rgb80C2=5445,rgb80E2=5446,rgb8202=5447,rgb8222=5448,rgb8242=5449,rgb8262=5450,rgb8282=5451,rgb82A2=5452,rgb82C2=5453,rgb82E2=5454,rgb8402=5455,rgb8422=5456,rgb8442=5457,rgb8462=5458,rgb8482=5459,rgb84A2=5460,rgb84C2=5461,rgb84E2=5462,rgb8602=5463,rgb8622=5464,rgb8642=5465,rgb8662=5466,rgb8682=5467,rgb86A2=5468,rgb86C2=5469,rgb86E2=5470,rgb8802=5471,rgb8822=5472,rgb8842=5473,rgb8862=5474,rgb8882=5475,rgb88A2=5476,rgb88C2=5477,rgb88E2=5478,rgb8A02=5479,rgb8A22=5480,rgb8A42=5481,rgb8A62=5482,rgb8A82=5483,rgb8AA2=5484,rgb8AC2=5485,rgb8AE2=5486,rgb8C02=5487,rgb8C22=5488,rgb8C42=5489,rgb8C62=5490,rgb8C82=5491,rgb8CA2=5492,rgb8CC2=5493,rgb8CE2=5494,rgb8E02=5495,rgb8E22=5496,rgb8E42=5497,rgb8E62=5498,rgb8E82=5499,rgb8EA2=5500,rgb8EC2=5501,rgb8EE2=5502,rgbA002=5503,rgbA022=5504,rgbA042=5505,rgbA062=5506,rgbA082=5507,rgbA0A2=5508,rgbA0C2=5509,rgbA0E2=5510,rgbA202=5511,rgbA222=5512,rgbA242=5513,rgbA262=5514,rgbA282=5515,rgbA2A2=5516,rgbA2C2=5517,rgbA2E2=5518,rgbA402=5519,rgbA422=5520,rgbA442=5521,rgbA462=5522,rgbA482=5523,rgbA4A2=5524,rgbA4C2=5525,rgbA4E2=5526,rgbA602=5527,rgbA622=5528,rgbA642=5529,rgbA662=5530,rgbA682=5531,rgbA6A2=5532,rgbA6C2=5533,rgbA6E2=5534,rgbA802=5535,rgbA822=5536,rgbA842=5537,rgbA862=5538,rgbA882=5539,rgbA8A2=5540,rgbA8C2=5541,rgbA8E2=5542,rgbAA02=5543,rgbAA22=5544,rgbAA42=5545,rgbAA62=5546,rgbAA82=5547,rgbAAA2=5548,rgbAAC2=5549,rgbAAE2=5550,rgbAC02=5551,rgbAC22=5552,rgbAC42=5553,rgbAC62=5554,rgbAC82=5555,rgbACA2=5556,rgbACC2=5557,rgbACE2=5558,rgbAE02=5559,rgbAE22=5560,rgbAE42=5561,rgbAE62=5562,rgbAE82=5563,rgbAEA2=5564,rgbAEC2=5565,rgbAEE2=5566,rgbC002=5567,rgbC022=5568,rgbC042=5569,rgbC062=5570,rgbC082=5571,rgbC0A2=5572,rgbC0C2=5573,rgbC0E2=5574,rgbC202=5575,rgbC222=5576,rgbC242=5577,rgbC262=5578,rgbC282=5579,rgbC2A2=5580,rgbC2C2=5581,rgbC2E2=5582,rgbC402=5583,rgbC422=5584,rgbC442=5585,rgbC462=5586,rgbC482=5587,rgbC4A2=5588,rgbC4C2=5589,rgbC4E2=5590,rgbC602=5591,rgbC622=5592,rgbC642=5593,rgbC662=5594,rgbC682=5595,rgbC6A2=5596,rgbC6C2=5597,rgbC6E2=5598,rgbC802=5599,rgbC822=5600,rgbC842=5601,
rgbC862=5602,rgbC882=5603,rgbC8A2=5604,rgbC8C2=5605,rgbC8E2=5606,rgbCA02=5607,rgbCA22=5608,rgbCA42=5609,rgbCA62=5610,rgbCA82=5611,rgbCAA2=5612,rgbCAC2=5613,rgbCAE2=5614,rgbCC02=5615,rgbCC22=5616,rgbCC42=5617,rgbCC62=5618,rgbCC82=5619,rgbCCA2=5620,rgbCCC2=5621,rgbCCE2=5622,rgbCE02=5623,rgbCE22=5624,rgbCE42=5625,rgbCE62=5626,rgbCE82=5627,rgbCEA2=5628,rgbCEC2=5629,rgbCEE2=5630,rgbE002=5631,rgbE022=5632,rgbE042=5633,rgbE062=5634,rgbE082=5635,rgbE0A2=5636,rgbE0C2=5637,rgbE0E2=5638,rgbE202=5639,rgbE222=5640,rgbE242=5641,rgbE262=5642,rgbE282=5643,rgbE2A2=5644,rgbE2C2=5645,rgbE2E2=5646,rgbE402=5647,rgbE422=5648,rgbE442=5649,rgbE462=5650,rgbE482=5651,rgbE4A2=5652,rgbE4C2=5653,rgbE4E2=5654,rgbE602=5655,rgbE622=5656,rgbE642=5657,rgbE662=5658,rgbE682=5659,rgbE6A2=5660,rgbE6C2=5661,rgbE6E2=5662,rgbE802=5663,rgbE822=5664,rgbE842=5665,rgbE862=5666,rgbE882=5667,rgbE8A2=5668,rgbE8C2=5669,rgbE8E2=5670,rgbEA02=5671,rgbEA22=5672,rgbEA42=5673,rgbEA62=5674,rgbEA82=5675,rgbEAA2=5676,rgbEAC2=5677,rgbEAE2=5678,rgbEC02=5679,rgbEC22=5680,rgbEC42=5681,rgbEC62=5682,rgbEC82=5683,rgbECA2=5684,rgbECC2=5685,rgbECE2=5686,rgbEE02=5687,rgbEE22=5688,rgbEE42=5689,rgbEE62=5690,rgbEE82=5691,rgbEEA2=5692,rgbEEC2=5693,rgbEEE2=5694,rgb0003=5695,rgb0023=5696,rgb0043=5697,rgb0063=5698,rgb0083=5699,rgb00A3=5700,rgb00C3=5701,rgb00E3=5702,rgb0203=5703,rgb0223=5704,rgb0243=5705,rgb0263=5706,rgb0283=5707,rgb02A3=5708,rgb02C3=5709,rgb02E3=5710,rgb0403=5711,rgb0423=5712,rgb0443=5713,rgb0463=5714,rgb0483=5715,rgb04A3=5716,rgb04C3=5717,rgb04E3=5718,rgb0603=5719,rgb0623=5720,rgb0643=5721,rgb0663=5722,rgb0683=5723,rgb06A3=5724,rgb06C3=5725,rgb06E3=5726,rgb0803=5727,rgb0823=5728,rgb0843=5729,rgb0863=5730,rgb0883=5731,rgb08A3=5732,rgb08C3=5733,rgb08E3=5734,rgb0A03=5735,rgb0A23=5736,rgb0A43=5737,rgb0A63=5738,rgb0A83=5739,rgb0AA3=5740,rgb0AC3=5741,rgb0AE3=5742,rgb0C03=5743,rgb0C23=5744,rgb0C43=5745,rgb0C63=5746,rgb0C83=5747,rgb0CA3=5748,rgb0CC3=5749,rgb0CE3=5750,rgb0E03=5751,rgb0E23=5752,rgb0E43=5753,rgb0E63=5754,rgb0E83=5755,rgb0EA3=5756,rgb0EC3=5757,rgb0EE3=5758,rgb2003=5759,rgb2023=5760,rgb2043=5761,rgb2063=5762,rgb2083=5763,rgb20A3=5764,rgb20C3=5765,rgb20E3=5766,rgb2203=5767,rgb2223=5768,rgb2243=5769,rgb2263=5770,rgb2283=5771,rgb22A3=5772,rgb22C3=5773,rgb22E3=5774,rgb2403=5775,rgb2423=5776,rgb2443=5777,
rgb2463=5778,rgb2483=5779,rgb24A3=5780,rgb24C3=5781,rgb24E3=5782,rgb2603=5783,rgb2623=5784,rgb2643=5785,rgb2663=5786,rgb2683=5787,rgb26A3=5788,rgb26C3=5789,rgb26E3=5790,rgb2803=5791,rgb2823=5792,rgb2843=5793,rgb2863=5794,rgb2883=5795,rgb28A3=5796,rgb28C3=5797,rgb28E3=5798,rgb2A03=5799,rgb2A23=5800,rgb2A43=5801,rgb2A63=5802,rgb2A83=5803,rgb2AA3=5804,rgb2AC3=5805,rgb2AE3=5806,rgb2C03=5807,rgb2C23=5808,rgb2C43=5809,rgb2C63=5810,rgb2C83=5811,rgb2CA3=5812,rgb2CC3=5813,rgb2CE3=5814,rgb2E03=5815,rgb2E23=5816,rgb2E43=5817,rgb2E63=5818,rgb2E83=5819,rgb2EA3=5820,rgb2EC3=5821,rgb2EE3=5822,rgb4003=5823,rgb4023=5824,rgb4043=5825,rgb4063=5826,rgb4083=5827,rgb40A3=5828,rgb40C3=5829,rgb40E3=5830,rgb4203=5831,rgb4223=5832,rgb4243=5833,rgb4263=5834,rgb4283=5835,rgb42A3=5836,rgb42C3=5837,rgb42E3=5838,rgb4403=5839,rgb4423=5840,rgb4443=5841,rgb4463=5842,rgb4483=5843,rgb44A3=5844,rgb44C3=5845,rgb44E3=5846,rgb4603=5847,rgb4623=5848,rgb4643=5849,rgb4663=5850,rgb4683=5851,rgb46A3=5852,rgb46C3=5853,rgb46E3=5854,rgb4803=5855,rgb4823=5856,rgb4843=5857,rgb4863=5858,rgb4883=5859,rgb48A3=5860,rgb48C3=5861,rgb48E3=5862,rgb4A03=5863,rgb4A23=5864,rgb4A43=5865,rgb4A63=5866,rgb4A83=5867,rgb4AA3=5868,rgb4AC3=5869,rgb4AE3=5870,rgb4C03=5871,rgb4C23=5872,rgb4C43=5873,rgb4C63=5874,rgb4C83=5875,rgb4CA3=5876,rgb4CC3=5877,rgb4CE3=5878,rgb4E03=5879,rgb4E23=5880,rgb4E43=5881,rgb4E63=5882,rgb4E83=5883,rgb4EA3=5884,rgb4EC3=5885,rgb4EE3=5886,rgb6003=5887,rgb6023=5888,rgb6043=5889,rgb6063=5890,rgb6083=5891,rgb60A3=5892,rgb60C3=5893,rgb60E3=5894,rgb6203=5895,rgb6223=5896,rgb6243=5897,rgb6263=5898,rgb6283=5899,rgb62A3=5900,rgb62C3=5901,rgb62E3=5902,rgb6403=5903,rgb6423=5904,rgb6443=5905,rgb6463=5906,rgb6483=5907,rgb64A3=5908,rgb64C3=5909,rgb64E3=5910,rgb6603=5911,rgb6623=5912,rgb6643=5913,rgb6663=5914,rgb6683=5915,rgb66A3=5916,rgb66C3=5917,rgb66E3=5918,rgb6803=5919,rgb6823=5920,rgb6843=5921,rgb6863=5922,rgb6883=5923,rgb68A3=5924,rgb68C3=5925,
rgb68E3=5926,rgb6A03=5927,rgb6A23=5928,rgb6A43=5929,rgb6A63=5930,rgb6A83=5931,rgb6AA3=5932,rgb6AC3=5933,rgb6AE3=5934,rgb6C03=5935,rgb6C23=5936,rgb6C43=5937,rgb6C63=5938,rgb6C83=5939,rgb6CA3=5940,rgb6CC3=5941,rgb6CE3=5942,rgb6E03=5943,rgb6E23=5944,rgb6E43=5945,rgb6E63=5946,rgb6E83=5947,rgb6EA3=5948,rgb6EC3=5949,rgb6EE3=5950,rgb8003=5951,rgb8023=5952,rgb8043=5953,rgb8063=5954,rgb8083=5955,rgb80A3=5956,rgb80C3=5957,rgb80E3=5958,rgb8203=5959,rgb8223=5960,rgb8243=5961,rgb8263=5962,rgb8283=5963,rgb82A3=5964,rgb82C3=5965,rgb82E3=5966,rgb8403=5967,rgb8423=5968,rgb8443=5969,rgb8463=5970,rgb8483=5971,rgb84A3=5972,rgb84C3=5973,rgb84E3=5974,rgb8603=5975,rgb8623=5976,rgb8643=5977,rgb8663=5978,rgb8683=5979,rgb86A3=5980,rgb86C3=5981,rgb86E3=5982,rgb8803=5983,rgb8823=5984,rgb8843=5985,rgb8863=5986,rgb8883=5987,rgb88A3=5988,rgb88C3=5989,rgb88E3=5990,rgb8A03=5991,rgb8A23=5992,rgb8A43=5993,rgb8A63=5994,rgb8A83=5995,rgb8AA3=5996,rgb8AC3=5997,rgb8AE3=5998,rgb8C03=5999,rgb8C23=6000,rgb8C43=6001,rgb8C63=6002,rgb8C83=6003,rgb8CA3=6004,rgb8CC3=6005,rgb8CE3=6006,rgb8E03=6007,rgb8E23=6008,rgb8E43=6009,rgb8E63=6010,rgb8E83=6011,rgb8EA3=6012,rgb8EC3=6013,rgb8EE3=6014,rgbA003=6015,rgbA023=6016,rgbA043=6017,rgbA063=6018,rgbA083=6019,rgbA0A3=6020,rgbA0C3=6021,rgbA0E3=6022,rgbA203=6023,rgbA223=6024,rgbA243=6025,rgbA263=6026,rgbA283=6027,rgbA2A3=6028,rgbA2C3=6029,rgbA2E3=6030,rgbA403=6031,rgbA423=6032,rgbA443=6033,rgbA463=6034,rgbA483=6035,rgbA4A3=6036,rgbA4C3=6037,rgbA4E3=6038,rgbA603=6039,rgbA623=6040,rgbA643=6041,rgbA663=6042,rgbA683=6043,rgbA6A3=6044,rgbA6C3=6045,rgbA6E3=6046,rgbA803=6047,rgbA823=6048,rgbA843=6049,rgbA863=6050,rgbA883=6051,rgbA8A3=6052,rgbA8C3=6053,rgbA8E3=6054,rgbAA03=6055,rgbAA23=6056,rgbAA43=6057,rgbAA63=6058,rgbAA83=6059,rgbAAA3=6060,rgbAAC3=6061,rgbAAE3=6062,rgbAC03=6063,rgbAC23=6064,rgbAC43=6065,rgbAC63=6066,rgbAC83=6067,rgbACA3=6068,rgbACC3=6069,rgbACE3=6070,rgbAE03=6071,rgbAE23=6072,rgbAE43=6073,rgbAE63=6074,rgbAE83=6075,rgbAEA3=6076,rgbAEC3=6077,rgbAEE3=6078,rgbC003=6079,rgbC023=6080,rgbC043=6081,rgbC063=6082,rgbC083=6083,rgbC0A3=6084,rgbC0C3=6085,rgbC0E3=6086,rgbC203=6087,rgbC223=6088,rgbC243=6089,rgbC263=6090,rgbC283=6091,rgbC2A3=6092,rgbC2C3=6093,rgbC2E3=6094,rgbC403=6095,rgbC423=6096,rgbC443=6097,rgbC463=6098,rgbC483=6099,rgbC4A3=6100,rgbC4C3=6101,
rgbC4E3=6102,rgbC603=6103,rgbC623=6104,rgbC643=6105,rgbC663=6106,rgbC683=6107,rgbC6A3=6108,rgbC6C3=6109,rgbC6E3=6110,rgbC803=6111,rgbC823=6112,rgbC843=6113,rgbC863=6114,rgbC883=6115,rgbC8A3=6116,rgbC8C3=6117,rgbC8E3=6118,rgbCA03=6119,rgbCA23=6120,rgbCA43=6121,rgbCA63=6122,rgbCA83=6123,rgbCAA3=6124,rgbCAC3=6125,rgbCAE3=6126,rgbCC03=6127,rgbCC23=6128,rgbCC43=6129,rgbCC63=6130,rgbCC83=6131,rgbCCA3=6132,rgbCCC3=6133,rgbCCE3=6134,rgbCE03=6135,rgbCE23=6136,rgbCE43=6137,rgbCE63=6138,rgbCE83=6139,rgbCEA3=6140,rgbCEC3=6141,rgbCEE3=6142,rgbE003=6143,rgbE023=6144,rgbE043=6145,rgbE063=6146,rgbE083=6147,rgbE0A3=6148,rgbE0C3=6149,rgbE0E3=6150,rgbE203=6151,rgbE223=6152,rgbE243=6153,rgbE263=6154,rgbE283=6155,rgbE2A3=6156,rgbE2C3=6157,rgbE2E3=6158,rgbE403=6159,rgbE423=6160,rgbE443=6161,rgbE463=6162,rgbE483=6163,rgbE4A3=6164,rgbE4C3=6165,rgbE4E3=6166,rgbE603=6167,rgbE623=6168,rgbE643=6169,rgbE663=6170,rgbE683=6171,rgbE6A3=6172,rgbE6C3=6173,rgbE6E3=6174,rgbE803=6175,rgbE823=6176,rgbE843=6177,rgbE863=6178,rgbE883=6179,rgbE8A3=6180,rgbE8C3=6181,rgbE8E3=6182,rgbEA03=6183,rgbEA23=6184,rgbEA43=6185,rgbEA63=6186,rgbEA83=6187,rgbEAA3=6188,rgbEAC3=6189,rgbEAE3=6190,rgbEC03=6191,rgbEC23=6192,rgbEC43=6193,rgbEC63=6194,rgbEC83=6195,rgbECA3=6196,rgbECC3=6197,rgbECE3=6198,rgbEE03=6199,rgbEE23=6200,rgbEE43=6201,rgbEE63=6202,rgbEE83=6203,rgbEEA3=6204,rgbEEC3=6205,rgbEEE3=6206,rgb0004=6207,rgb0024=6208,rgb0044=6209,rgb0064=6210,rgb0084=6211,rgb00A4=6212,rgb00C4=6213,rgb00E4=6214,rgb0204=6215,rgb0224=6216,rgb0244=6217,rgb0264=6218,rgb0284=6219,rgb02A4=6220,rgb02C4=6221,rgb02E4=6222,rgb0404=6223,rgb0424=6224,rgb0444=6225,rgb0464=6226,rgb0484=6227,rgb04A4=6228,rgb04C4=6229,rgb04E4=6230,rgb0604=6231,rgb0624=6232,rgb0644=6233,rgb0664=6234,rgb0684=6235,rgb06A4=6236,rgb06C4=6237,rgb06E4=6238,rgb0804=6239,rgb0824=6240,rgb0844=6241,rgb0864=6242,rgb0884=6243,rgb08A4=6244,rgb08C4=6245,rgb08E4=6246,rgb0A04=6247,rgb0A24=6248,rgb0A44=6249,rgb0A64=6250,rgb0A84=6251,rgb0AA4=6252,rgb0AC4=6253,rgb0AE4=6254,rgb0C04=6255,rgb0C24=6256,rgb0C44=6257,rgb0C64=6258,rgb0C84=6259,rgb0CA4=6260,rgb0CC4=6261,rgb0CE4=6262,rgb0E04=6263,rgb0E24=6264,rgb0E44=6265,rgb0E64=6266,rgb0E84=6267,rgb0EA4=6268,rgb0EC4=6269,rgb0EE4=6270,rgb2004=6271,rgb2024=6272,rgb2044=6273,rgb2064=6274,rgb2084=6275,rgb20A4=6276,rgb20C4=6277,
rgb20E4=6278,rgb2204=6279,rgb2224=6280,rgb2244=6281,rgb2264=6282,rgb2284=6283,rgb22A4=6284,rgb22C4=6285,rgb22E4=6286,rgb2404=6287,rgb2424=6288,rgb2444=6289,rgb2464=6290,rgb2484=6291,rgb24A4=6292,rgb24C4=6293,rgb24E4=6294,rgb2604=6295,rgb2624=6296,rgb2644=6297,rgb2664=6298,rgb2684=6299,rgb26A4=6300,rgb26C4=6301,rgb26E4=6302,rgb2804=6303,rgb2824=6304,rgb2844=6305,rgb2864=6306,rgb2884=6307,rgb28A4=6308,rgb28C4=6309,rgb28E4=6310,rgb2A04=6311,rgb2A24=6312,rgb2A44=6313,rgb2A64=6314,rgb2A84=6315,rgb2AA4=6316,rgb2AC4=6317,rgb2AE4=6318,rgb2C04=6319,rgb2C24=6320,rgb2C44=6321,rgb2C64=6322,rgb2C84=6323,rgb2CA4=6324,rgb2CC4=6325,rgb2CE4=6326,rgb2E04=6327,rgb2E24=6328,rgb2E44=6329,rgb2E64=6330,rgb2E84=6331,rgb2EA4=6332,rgb2EC4=6333,rgb2EE4=6334,rgb4004=6335,rgb4024=6336,rgb4044=6337,rgb4064=6338,rgb4084=6339,rgb40A4=6340,rgb40C4=6341,rgb40E4=6342,rgb4204=6343,rgb4224=6344,rgb4244=6345,rgb4264=6346,rgb4284=6347,rgb42A4=6348,rgb42C4=6349,rgb42E4=6350,rgb4404=6351,rgb4424=6352,rgb4444=6353,rgb4464=6354,rgb4484=6355,rgb44A4=6356,rgb44C4=6357,rgb44E4=6358,rgb4604=6359,rgb4624=6360,rgb4644=6361,rgb4664=6362,rgb4684=6363,rgb46A4=6364,rgb46C4=6365,rgb46E4=6366,rgb4804=6367,rgb4824=6368,rgb4844=6369,rgb4864=6370,rgb4884=6371,rgb48A4=6372,rgb48C4=6373,rgb48E4=6374,rgb4A04=6375,rgb4A24=6376,rgb4A44=6377,rgb4A64=6378,rgb4A84=6379,rgb4AA4=6380,rgb4AC4=6381,rgb4AE4=6382,rgb4C04=6383,rgb4C24=6384,rgb4C44=6385,rgb4C64=6386,rgb4C84=6387,rgb4CA4=6388,rgb4CC4=6389,rgb4CE4=6390,rgb4E04=6391,rgb4E24=6392,rgb4E44=6393,rgb4E64=6394,rgb4E84=6395,rgb4EA4=6396,rgb4EC4=6397,rgb4EE4=6398,rgb6004=6399,rgb6024=6400,rgb6044=6401,rgb6064=6402,rgb6084=6403,rgb60A4=6404,rgb60C4=6405,rgb60E4=6406,rgb6204=6407,rgb6224=6408,rgb6244=6409,rgb6264=6410,rgb6284=6411,rgb62A4=6412,rgb62C4=6413,rgb62E4=6414,rgb6404=6415,rgb6424=6416,rgb6444=6417,rgb6464=6418,rgb6484=6419,rgb64A4=6420,rgb64C4=6421,rgb64E4=6422,rgb6604=6423,rgb6624=6424,rgb6644=6425,rgb6664=6426,rgb6684=6427,rgb66A4=6428,rgb66C4=6429,rgb66E4=6430,rgb6804=6431,rgb6824=6432,rgb6844=6433,rgb6864=6434,rgb6884=6435,rgb68A4=6436,rgb68C4=6437,rgb68E4=6438,rgb6A04=6439,rgb6A24=6440,rgb6A44=6441,rgb6A64=6442,rgb6A84=6443,rgb6AA4=6444,rgb6AC4=6445,rgb6AE4=6446,rgb6C04=6447,rgb6C24=6448,rgb6C44=6449,rgb6C64=6450,rgb6C84=6451,rgb6CA4=6452,rgb6CC4=6453,
rgb6CE4=6454,rgb6E04=6455,rgb6E24=6456,rgb6E44=6457,rgb6E64=6458,rgb6E84=6459,rgb6EA4=6460,rgb6EC4=6461,rgb6EE4=6462,rgb8004=6463,rgb8024=6464,rgb8044=6465,rgb8064=6466,rgb8084=6467,rgb80A4=6468,rgb80C4=6469,rgb80E4=6470,rgb8204=6471,rgb8224=6472,rgb8244=6473,rgb8264=6474,rgb8284=6475,rgb82A4=6476,rgb82C4=6477,rgb82E4=6478,rgb8404=6479,rgb8424=6480,rgb8444=6481,rgb8464=6482,rgb8484=6483,rgb84A4=6484,rgb84C4=6485,rgb84E4=6486,rgb8604=6487,rgb8624=6488,rgb8644=6489,rgb8664=6490,rgb8684=6491,rgb86A4=6492,rgb86C4=6493,rgb86E4=6494,rgb8804=6495,rgb8824=6496,rgb8844=6497,rgb8864=6498,rgb8884=6499,rgb88A4=6500,rgb88C4=6501,rgb88E4=6502,rgb8A04=6503,rgb8A24=6504,rgb8A44=6505,rgb8A64=6506,rgb8A84=6507,rgb8AA4=6508,rgb8AC4=6509,rgb8AE4=6510,rgb8C04=6511,rgb8C24=6512,rgb8C44=6513,rgb8C64=6514,rgb8C84=6515,rgb8CA4=6516,rgb8CC4=6517,rgb8CE4=6518,rgb8E04=6519,rgb8E24=6520,rgb8E44=6521,rgb8E64=6522,rgb8E84=6523,rgb8EA4=6524,rgb8EC4=6525,rgb8EE4=6526,rgbA004=6527,rgbA024=6528,rgbA044=6529,rgbA064=6530,rgbA084=6531,rgbA0A4=6532,rgbA0C4=6533,rgbA0E4=6534,rgbA204=6535,rgbA224=6536,rgbA244=6537,rgbA264=6538,rgbA284=6539,rgbA2A4=6540,rgbA2C4=6541,rgbA2E4=6542,rgbA404=6543,rgbA424=6544,rgbA444=6545,rgbA464=6546,rgbA484=6547,rgbA4A4=6548,rgbA4C4=6549,rgbA4E4=6550,rgbA604=6551,rgbA624=6552,rgbA644=6553,rgbA664=6554,rgbA684=6555,rgbA6A4=6556,rgbA6C4=6557,rgbA6E4=6558,rgbA804=6559,rgbA824=6560,rgbA844=6561,rgbA864=6562,rgbA884=6563,rgbA8A4=6564,rgbA8C4=6565,rgbA8E4=6566,rgbAA04=6567,rgbAA24=6568,rgbAA44=6569,rgbAA64=6570,rgbAA84=6571,rgbAAA4=6572,rgbAAC4=6573,rgbAAE4=6574,rgbAC04=6575,rgbAC24=6576,rgbAC44=6577,rgbAC64=6578,rgbAC84=6579,rgbACA4=6580,rgbACC4=6581,rgbACE4=6582,rgbAE04=6583,rgbAE24=6584,rgbAE44=6585,rgbAE64=6586,rgbAE84=6587,rgbAEA4=6588,rgbAEC4=6589,rgbAEE4=6590,rgbC004=6591,rgbC024=6592,rgbC044=6593,rgbC064=6594,rgbC084=6595,rgbC0A4=6596,rgbC0C4=6597,rgbC0E4=6598,rgbC204=6599,rgbC224=6600,rgbC244=6601,
rgbC264=6602,rgbC284=6603,rgbC2A4=6604,rgbC2C4=6605,rgbC2E4=6606,rgbC404=6607,rgbC424=6608,rgbC444=6609,rgbC464=6610,rgbC484=6611,rgbC4A4=6612,rgbC4C4=6613,rgbC4E4=6614,rgbC604=6615,rgbC624=6616,rgbC644=6617,rgbC664=6618,rgbC684=6619,rgbC6A4=6620,rgbC6C4=6621,rgbC6E4=6622,rgbC804=6623,rgbC824=6624,rgbC844=6625,rgbC864=6626,rgbC884=6627,rgbC8A4=6628,rgbC8C4=6629,rgbC8E4=6630,rgbCA04=6631,rgbCA24=6632,rgbCA44=6633,rgbCA64=6634,rgbCA84=6635,rgbCAA4=6636,rgbCAC4=6637,rgbCAE4=6638,rgbCC04=6639,rgbCC24=6640,rgbCC44=6641,rgbCC64=6642,rgbCC84=6643,rgbCCA4=6644,rgbCCC4=6645,rgbCCE4=6646,rgbCE04=6647,rgbCE24=6648,rgbCE44=6649,rgbCE64=6650,rgbCE84=6651,rgbCEA4=6652,rgbCEC4=6653,rgbCEE4=6654,rgbE004=6655,rgbE024=6656,rgbE044=6657,rgbE064=6658,rgbE084=6659,rgbE0A4=6660,rgbE0C4=6661,rgbE0E4=6662,rgbE204=6663,rgbE224=6664,rgbE244=6665,rgbE264=6666,rgbE284=6667,rgbE2A4=6668,rgbE2C4=6669,rgbE2E4=6670,rgbE404=6671,rgbE424=6672,rgbE444=6673,rgbE464=6674,rgbE484=6675,rgbE4A4=6676,rgbE4C4=6677,rgbE4E4=6678,rgbE604=6679,rgbE624=6680,rgbE644=6681,rgbE664=6682,rgbE684=6683,rgbE6A4=6684,rgbE6C4=6685,rgbE6E4=6686,rgbE804=6687,rgbE824=6688,rgbE844=6689,rgbE864=6690,rgbE884=6691,rgbE8A4=6692,rgbE8C4=6693,rgbE8E4=6694,rgbEA04=6695,rgbEA24=6696,rgbEA44=6697,rgbEA64=6698,rgbEA84=6699,rgbEAA4=6700,rgbEAC4=6701,rgbEAE4=6702,rgbEC04=6703,rgbEC24=6704,rgbEC44=6705,rgbEC64=6706,rgbEC84=6707,rgbECA4=6708,rgbECC4=6709,rgbECE4=6710,rgbEE04=6711,rgbEE24=6712,rgbEE44=6713,rgbEE64=6714,rgbEE84=6715,rgbEEA4=6716,rgbEEC4=6717,rgbEEE4=6718,rgb0005=6719,rgb0025=6720,rgb0045=6721,rgb0065=6722,rgb0085=6723,rgb00A5=6724,rgb00C5=6725,rgb00E5=6726,rgb0205=6727,rgb0225=6728,rgb0245=6729,rgb0265=6730,rgb0285=6731,rgb02A5=6732,rgb02C5=6733,rgb02E5=6734,rgb0405=6735,rgb0425=6736,rgb0445=6737,rgb0465=6738,rgb0485=6739,rgb04A5=6740,rgb04C5=6741,rgb04E5=6742,rgb0605=6743,rgb0625=6744,rgb0645=6745,rgb0665=6746,rgb0685=6747,rgb06A5=6748,rgb06C5=6749,rgb06E5=6750,rgb0805=6751,rgb0825=6752,rgb0845=6753,rgb0865=6754,rgb0885=6755,rgb08A5=6756,rgb08C5=6757,rgb08E5=6758,rgb0A05=6759,rgb0A25=6760,rgb0A45=6761,rgb0A65=6762,rgb0A85=6763,rgb0AA5=6764,rgb0AC5=6765,rgb0AE5=6766,rgb0C05=6767,rgb0C25=6768,rgb0C45=6769,rgb0C65=6770,rgb0C85=6771,rgb0CA5=6772,rgb0CC5=6773,rgb0CE5=6774,rgb0E05=6775,rgb0E25=6776,rgb0E45=6777,
rgb0E65=6778,rgb0E85=6779,rgb0EA5=6780,rgb0EC5=6781,rgb0EE5=6782,rgb2005=6783,rgb2025=6784,rgb2045=6785,rgb2065=6786,rgb2085=6787,rgb20A5=6788,rgb20C5=6789,rgb20E5=6790,rgb2205=6791,rgb2225=6792,rgb2245=6793,rgb2265=6794,rgb2285=6795,rgb22A5=6796,rgb22C5=6797,rgb22E5=6798,rgb2405=6799,rgb2425=6800,rgb2445=6801,rgb2465=6802,rgb2485=6803,rgb24A5=6804,rgb24C5=6805,rgb24E5=6806,rgb2605=6807,rgb2625=6808,rgb2645=6809,rgb2665=6810,rgb2685=6811,rgb26A5=6812,rgb26C5=6813,rgb26E5=6814,rgb2805=6815,rgb2825=6816,rgb2845=6817,rgb2865=6818,rgb2885=6819,rgb28A5=6820,rgb28C5=6821,rgb28E5=6822,rgb2A05=6823,rgb2A25=6824,rgb2A45=6825,rgb2A65=6826,rgb2A85=6827,rgb2AA5=6828,rgb2AC5=6829,rgb2AE5=6830,rgb2C05=6831,rgb2C25=6832,rgb2C45=6833,rgb2C65=6834,rgb2C85=6835,rgb2CA5=6836,rgb2CC5=6837,rgb2CE5=6838,rgb2E05=6839,rgb2E25=6840,rgb2E45=6841,rgb2E65=6842,rgb2E85=6843,rgb2EA5=6844,rgb2EC5=6845,rgb2EE5=6846,rgb4005=6847,rgb4025=6848,rgb4045=6849,rgb4065=6850,rgb4085=6851,rgb40A5=6852,rgb40C5=6853,rgb40E5=6854,rgb4205=6855,rgb4225=6856,rgb4245=6857,rgb4265=6858,rgb4285=6859,rgb42A5=6860,rgb42C5=6861,rgb42E5=6862,rgb4405=6863,rgb4425=6864,rgb4445=6865,rgb4465=6866,rgb4485=6867,rgb44A5=6868,rgb44C5=6869,rgb44E5=6870,rgb4605=6871,rgb4625=6872,rgb4645=6873,rgb4665=6874,rgb4685=6875,rgb46A5=6876,rgb46C5=6877,rgb46E5=6878,rgb4805=6879,rgb4825=6880,rgb4845=6881,rgb4865=6882,rgb4885=6883,rgb48A5=6884,rgb48C5=6885,rgb48E5=6886,rgb4A05=6887,rgb4A25=6888,rgb4A45=6889,rgb4A65=6890,rgb4A85=6891,rgb4AA5=6892,rgb4AC5=6893,rgb4AE5=6894,rgb4C05=6895,rgb4C25=6896,rgb4C45=6897,rgb4C65=6898,rgb4C85=6899,rgb4CA5=6900,rgb4CC5=6901,rgb4CE5=6902,rgb4E05=6903,rgb4E25=6904,rgb4E45=6905,rgb4E65=6906,rgb4E85=6907,rgb4EA5=6908,rgb4EC5=6909,rgb4EE5=6910,rgb6005=6911,rgb6025=6912,rgb6045=6913,rgb6065=6914,rgb6085=6915,rgb60A5=6916,rgb60C5=6917,rgb60E5=6918,rgb6205=6919,rgb6225=6920,rgb6245=6921,rgb6265=6922,rgb6285=6923,rgb62A5=6924,rgb62C5=6925,rgb62E5=6926,rgb6405=6927,rgb6425=6928,rgb6445=6929,rgb6465=6930,rgb6485=6931,rgb64A5=6932,rgb64C5=6933,rgb64E5=6934,rgb6605=6935,rgb6625=6936,rgb6645=6937,rgb6665=6938,rgb6685=6939,rgb66A5=6940,rgb66C5=6941,rgb66E5=6942,rgb6805=6943,rgb6825=6944,rgb6845=6945,rgb6865=6946,rgb6885=6947,rgb68A5=6948,rgb68C5=6949,rgb68E5=6950,rgb6A05=6951,rgb6A25=6952,rgb6A45=6953,
rgb6A65=6954,rgb6A85=6955,rgb6AA5=6956,rgb6AC5=6957,rgb6AE5=6958,rgb6C05=6959,rgb6C25=6960,rgb6C45=6961,rgb6C65=6962,rgb6C85=6963,rgb6CA5=6964,rgb6CC5=6965,rgb6CE5=6966,rgb6E05=6967,rgb6E25=6968,rgb6E45=6969,rgb6E65=6970,rgb6E85=6971,rgb6EA5=6972,rgb6EC5=6973,rgb6EE5=6974,rgb8005=6975,rgb8025=6976,rgb8045=6977,rgb8065=6978,rgb8085=6979,rgb80A5=6980,rgb80C5=6981,rgb80E5=6982,rgb8205=6983,rgb8225=6984,rgb8245=6985,rgb8265=6986,rgb8285=6987,rgb82A5=6988,rgb82C5=6989,rgb82E5=6990,rgb8405=6991,rgb8425=6992,rgb8445=6993,rgb8465=6994,rgb8485=6995,rgb84A5=6996,rgb84C5=6997,rgb84E5=6998,rgb8605=6999,rgb8625=7000,rgb8645=7001,rgb8665=7002,rgb8685=7003,rgb86A5=7004,rgb86C5=7005,rgb86E5=7006,rgb8805=7007,rgb8825=7008,rgb8845=7009,rgb8865=7010,rgb8885=7011,rgb88A5=7012,rgb88C5=7013,rgb88E5=7014,rgb8A05=7015,rgb8A25=7016,rgb8A45=7017,rgb8A65=7018,rgb8A85=7019,rgb8AA5=7020,rgb8AC5=7021,rgb8AE5=7022,rgb8C05=7023,rgb8C25=7024,rgb8C45=7025,rgb8C65=7026,rgb8C85=7027,rgb8CA5=7028,rgb8CC5=7029,rgb8CE5=7030,rgb8E05=7031,rgb8E25=7032,rgb8E45=7033,rgb8E65=7034,rgb8E85=7035,rgb8EA5=7036,rgb8EC5=7037,rgb8EE5=7038,rgbA005=7039,rgbA025=7040,rgbA045=7041,rgbA065=7042,rgbA085=7043,rgbA0A5=7044,rgbA0C5=7045,rgbA0E5=7046,rgbA205=7047,rgbA225=7048,rgbA245=7049,rgbA265=7050,rgbA285=7051,rgbA2A5=7052,rgbA2C5=7053,rgbA2E5=7054,rgbA405=7055,rgbA425=7056,rgbA445=7057,rgbA465=7058,rgbA485=7059,rgbA4A5=7060,rgbA4C5=7061,rgbA4E5=7062,rgbA605=7063,rgbA625=7064,rgbA645=7065,rgbA665=7066,rgbA685=7067,rgbA6A5=7068,rgbA6C5=7069,rgbA6E5=7070,rgbA805=7071,rgbA825=7072,rgbA845=7073,rgbA865=7074,rgbA885=7075,rgbA8A5=7076,rgbA8C5=7077,rgbA8E5=7078,rgbAA05=7079,rgbAA25=7080,rgbAA45=7081,rgbAA65=7082,rgbAA85=7083,rgbAAA5=7084,rgbAAC5=7085,rgbAAE5=7086,rgbAC05=7087,rgbAC25=7088,rgbAC45=7089,rgbAC65=7090,rgbAC85=7091,rgbACA5=7092,rgbACC5=7093,rgbACE5=7094,rgbAE05=7095,rgbAE25=7096,rgbAE45=7097,rgbAE65=7098,rgbAE85=7099,rgbAEA5=7100,rgbAEC5=7101,rgbAEE5=7102,rgbC005=7103,rgbC025=7104,rgbC045=7105,rgbC065=7106,rgbC085=7107,rgbC0A5=7108,rgbC0C5=7109,rgbC0E5=7110,rgbC205=7111,rgbC225=7112,rgbC245=7113,rgbC265=7114,rgbC285=7115,rgbC2A5=7116,rgbC2C5=7117,rgbC2E5=7118,rgbC405=7119,rgbC425=7120,rgbC445=7121,rgbC465=7122,rgbC485=7123,rgbC4A5=7124,rgbC4C5=7125,rgbC4E5=7126,rgbC605=7127,rgbC625=7128,rgbC645=7129,
rgbC665=7130,rgbC685=7131,rgbC6A5=7132,rgbC6C5=7133,rgbC6E5=7134,rgbC805=7135,rgbC825=7136,rgbC845=7137,rgbC865=7138,rgbC885=7139,rgbC8A5=7140,rgbC8C5=7141,rgbC8E5=7142,rgbCA05=7143,rgbCA25=7144,rgbCA45=7145,rgbCA65=7146,rgbCA85=7147,rgbCAA5=7148,rgbCAC5=7149,rgbCAE5=7150,rgbCC05=7151,rgbCC25=7152,rgbCC45=7153,rgbCC65=7154,rgbCC85=7155,rgbCCA5=7156,rgbCCC5=7157,rgbCCE5=7158,rgbCE05=7159,rgbCE25=7160,rgbCE45=7161,rgbCE65=7162,rgbCE85=7163,rgbCEA5=7164,rgbCEC5=7165,rgbCEE5=7166,rgbE005=7167,rgbE025=7168,rgbE045=7169,rgbE065=7170,rgbE085=7171,rgbE0A5=7172,rgbE0C5=7173,rgbE0E5=7174,rgbE205=7175,rgbE225=7176,rgbE245=7177,rgbE265=7178,rgbE285=7179,rgbE2A5=7180,rgbE2C5=7181,rgbE2E5=7182,rgbE405=7183,rgbE425=7184,rgbE445=7185,rgbE465=7186,rgbE485=7187,rgbE4A5=7188,rgbE4C5=7189,rgbE4E5=7190,rgbE605=7191,rgbE625=7192,rgbE645=7193,rgbE665=7194,rgbE685=7195,rgbE6A5=7196,rgbE6C5=7197,rgbE6E5=7198,rgbE805=7199,rgbE825=7200,rgbE845=7201,rgbE865=7202,rgbE885=7203,rgbE8A5=7204,rgbE8C5=7205,rgbE8E5=7206,rgbEA05=7207,rgbEA25=7208,rgbEA45=7209,rgbEA65=7210,rgbEA85=7211,rgbEAA5=7212,rgbEAC5=7213,rgbEAE5=7214,rgbEC05=7215,rgbEC25=7216,rgbEC45=7217,rgbEC65=7218,rgbEC85=7219,rgbECA5=7220,rgbECC5=7221,rgbECE5=7222,rgbEE05=7223,rgbEE25=7224,rgbEE45=7225,rgbEE65=7226,rgbEE85=7227,rgbEEA5=7228,rgbEEC5=7229,rgbEEE5=7230,rgb0006=7231,rgb0026=7232,rgb0046=7233,rgb0066=7234,rgb0086=7235,rgb00A6=7236,rgb00C6=7237,rgb00E6=7238,rgb0206=7239,rgb0226=7240,rgb0246=7241,rgb0266=7242,rgb0286=7243,rgb02A6=7244,rgb02C6=7245,rgb02E6=7246,rgb0406=7247,rgb0426=7248,rgb0446=7249,rgb0466=7250,rgb0486=7251,rgb04A6=7252,rgb04C6=7253,rgb04E6=7254,rgb0606=7255,rgb0626=7256,rgb0646=7257,rgb0666=7258,rgb0686=7259,rgb06A6=7260,rgb06C6=7261,rgb06E6=7262,rgb0806=7263,rgb0826=7264,rgb0846=7265,rgb0866=7266,rgb0886=7267,rgb08A6=7268,rgb08C6=7269,rgb08E6=7270,rgb0A06=7271,rgb0A26=7272,rgb0A46=7273,rgb0A66=7274,rgb0A86=7275,rgb0AA6=7276,rgb0AC6=7277,
rgb0AE6=7278,rgb0C06=7279,rgb0C26=7280,rgb0C46=7281,rgb0C66=7282,rgb0C86=7283,rgb0CA6=7284,rgb0CC6=7285,rgb0CE6=7286,rgb0E06=7287,rgb0E26=7288,rgb0E46=7289,rgb0E66=7290,rgb0E86=7291,rgb0EA6=7292,rgb0EC6=7293,rgb0EE6=7294,rgb2006=7295,rgb2026=7296,rgb2046=7297,rgb2066=7298,rgb2086=7299,rgb20A6=7300,rgb20C6=7301,rgb20E6=7302,rgb2206=7303,rgb2226=7304,rgb2246=7305,rgb2266=7306,rgb2286=7307,rgb22A6=7308,rgb22C6=7309,rgb22E6=7310,rgb2406=7311,rgb2426=7312,rgb2446=7313,rgb2466=7314,rgb2486=7315,rgb24A6=7316,rgb24C6=7317,rgb24E6=7318,rgb2606=7319,rgb2626=7320,rgb2646=7321,rgb2666=7322,rgb2686=7323,rgb26A6=7324,rgb26C6=7325,rgb26E6=7326,rgb2806=7327,rgb2826=7328,rgb2846=7329,rgb2866=7330,rgb2886=7331,rgb28A6=7332,rgb28C6=7333,rgb28E6=7334,rgb2A06=7335,rgb2A26=7336,rgb2A46=7337,rgb2A66=7338,rgb2A86=7339,rgb2AA6=7340,rgb2AC6=7341,rgb2AE6=7342,rgb2C06=7343,rgb2C26=7344,rgb2C46=7345,rgb2C66=7346,rgb2C86=7347,rgb2CA6=7348,rgb2CC6=7349,rgb2CE6=7350,rgb2E06=7351,rgb2E26=7352,rgb2E46=7353,rgb2E66=7354,rgb2E86=7355,rgb2EA6=7356,rgb2EC6=7357,rgb2EE6=7358,rgb4006=7359,rgb4026=7360,rgb4046=7361,rgb4066=7362,rgb4086=7363,rgb40A6=7364,rgb40C6=7365,rgb40E6=7366,rgb4206=7367,rgb4226=7368,rgb4246=7369,rgb4266=7370,rgb4286=7371,rgb42A6=7372,rgb42C6=7373,rgb42E6=7374,rgb4406=7375,rgb4426=7376,rgb4446=7377,rgb4466=7378,rgb4486=7379,rgb44A6=7380,rgb44C6=7381,rgb44E6=7382,rgb4606=7383,rgb4626=7384,rgb4646=7385,rgb4666=7386,rgb4686=7387,rgb46A6=7388,rgb46C6=7389,rgb46E6=7390,rgb4806=7391,rgb4826=7392,rgb4846=7393,rgb4866=7394,rgb4886=7395,rgb48A6=7396,rgb48C6=7397,rgb48E6=7398,rgb4A06=7399,rgb4A26=7400,rgb4A46=7401,rgb4A66=7402,rgb4A86=7403,rgb4AA6=7404,rgb4AC6=7405,rgb4AE6=7406,rgb4C06=7407,rgb4C26=7408,rgb4C46=7409,rgb4C66=7410,rgb4C86=7411,rgb4CA6=7412,rgb4CC6=7413,rgb4CE6=7414,rgb4E06=7415,rgb4E26=7416,rgb4E46=7417,rgb4E66=7418,rgb4E86=7419,rgb4EA6=7420,rgb4EC6=7421,rgb4EE6=7422,rgb6006=7423,rgb6026=7424,rgb6046=7425,rgb6066=7426,rgb6086=7427,rgb60A6=7428,rgb60C6=7429,rgb60E6=7430,rgb6206=7431,rgb6226=7432,rgb6246=7433,rgb6266=7434,rgb6286=7435,rgb62A6=7436,rgb62C6=7437,rgb62E6=7438,rgb6406=7439,rgb6426=7440,rgb6446=7441,rgb6466=7442,rgb6486=7443,rgb64A6=7444,rgb64C6=7445,rgb64E6=7446,rgb6606=7447,rgb6626=7448,rgb6646=7449,rgb6666=7450,rgb6686=7451,rgb66A6=7452,rgb66C6=7453,
rgb66E6=7454,rgb6806=7455,rgb6826=7456,rgb6846=7457,rgb6866=7458,rgb6886=7459,rgb68A6=7460,rgb68C6=7461,rgb68E6=7462,rgb6A06=7463,rgb6A26=7464,rgb6A46=7465,rgb6A66=7466,rgb6A86=7467,rgb6AA6=7468,rgb6AC6=7469,rgb6AE6=7470,rgb6C06=7471,rgb6C26=7472,rgb6C46=7473,rgb6C66=7474,rgb6C86=7475,rgb6CA6=7476,rgb6CC6=7477,rgb6CE6=7478,rgb6E06=7479,rgb6E26=7480,rgb6E46=7481,rgb6E66=7482,rgb6E86=7483,rgb6EA6=7484,rgb6EC6=7485,rgb6EE6=7486,rgb8006=7487,rgb8026=7488,rgb8046=7489,rgb8066=7490,rgb8086=7491,rgb80A6=7492,rgb80C6=7493,rgb80E6=7494,rgb8206=7495,rgb8226=7496,rgb8246=7497,rgb8266=7498,rgb8286=7499,rgb82A6=7500,rgb82C6=7501,rgb82E6=7502,rgb8406=7503,rgb8426=7504,rgb8446=7505,rgb8466=7506,rgb8486=7507,rgb84A6=7508,rgb84C6=7509,rgb84E6=7510,rgb8606=7511,rgb8626=7512,rgb8646=7513,rgb8666=7514,rgb8686=7515,rgb86A6=7516,rgb86C6=7517,rgb86E6=7518,rgb8806=7519,rgb8826=7520,rgb8846=7521,rgb8866=7522,rgb8886=7523,rgb88A6=7524,rgb88C6=7525,rgb88E6=7526,rgb8A06=7527,rgb8A26=7528,rgb8A46=7529,rgb8A66=7530,rgb8A86=7531,rgb8AA6=7532,rgb8AC6=7533,rgb8AE6=7534,rgb8C06=7535,rgb8C26=7536,rgb8C46=7537,rgb8C66=7538,rgb8C86=7539,rgb8CA6=7540,rgb8CC6=7541,rgb8CE6=7542,rgb8E06=7543,rgb8E26=7544,rgb8E46=7545,rgb8E66=7546,rgb8E86=7547,rgb8EA6=7548,rgb8EC6=7549,rgb8EE6=7550,rgbA006=7551,rgbA026=7552,rgbA046=7553,rgbA066=7554,rgbA086=7555,rgbA0A6=7556,rgbA0C6=7557,rgbA0E6=7558,rgbA206=7559,rgbA226=7560,rgbA246=7561,rgbA266=7562,rgbA286=7563,rgbA2A6=7564,rgbA2C6=7565,rgbA2E6=7566,rgbA406=7567,rgbA426=7568,rgbA446=7569,rgbA466=7570,rgbA486=7571,rgbA4A6=7572,rgbA4C6=7573,rgbA4E6=7574,rgbA606=7575,rgbA626=7576,rgbA646=7577,rgbA666=7578,rgbA686=7579,rgbA6A6=7580,rgbA6C6=7581,rgbA6E6=7582,rgbA806=7583,rgbA826=7584,rgbA846=7585,rgbA866=7586,rgbA886=7587,rgbA8A6=7588,rgbA8C6=7589,rgbA8E6=7590,rgbAA06=7591,rgbAA26=7592,rgbAA46=7593,rgbAA66=7594,rgbAA86=7595,rgbAAA6=7596,rgbAAC6=7597,rgbAAE6=7598,rgbAC06=7599,rgbAC26=7600,rgbAC46=7601,
rgbAC66=7602,rgbAC86=7603,rgbACA6=7604,rgbACC6=7605,rgbACE6=7606,rgbAE06=7607,rgbAE26=7608,rgbAE46=7609,rgbAE66=7610,rgbAE86=7611,rgbAEA6=7612,rgbAEC6=7613,rgbAEE6=7614,rgbC006=7615,rgbC026=7616,rgbC046=7617,rgbC066=7618,rgbC086=7619,rgbC0A6=7620,rgbC0C6=7621,rgbC0E6=7622,rgbC206=7623,rgbC226=7624,rgbC246=7625,rgbC266=7626,rgbC286=7627,rgbC2A6=7628,rgbC2C6=7629,rgbC2E6=7630,rgbC406=7631,rgbC426=7632,rgbC446=7633,rgbC466=7634,rgbC486=7635,rgbC4A6=7636,rgbC4C6=7637,rgbC4E6=7638,rgbC606=7639,rgbC626=7640,rgbC646=7641,rgbC666=7642,rgbC686=7643,rgbC6A6=7644,rgbC6C6=7645,rgbC6E6=7646,rgbC806=7647,rgbC826=7648,rgbC846=7649,rgbC866=7650,rgbC886=7651,rgbC8A6=7652,rgbC8C6=7653,rgbC8E6=7654,rgbCA06=7655,rgbCA26=7656,rgbCA46=7657,rgbCA66=7658,rgbCA86=7659,rgbCAA6=7660,rgbCAC6=7661,rgbCAE6=7662,rgbCC06=7663,rgbCC26=7664,rgbCC46=7665,rgbCC66=7666,rgbCC86=7667,rgbCCA6=7668,rgbCCC6=7669,rgbCCE6=7670,rgbCE06=7671,rgbCE26=7672,rgbCE46=7673,rgbCE66=7674,rgbCE86=7675,rgbCEA6=7676,rgbCEC6=7677,rgbCEE6=7678,rgbE006=7679,rgbE026=7680,rgbE046=7681,rgbE066=7682,rgbE086=7683,rgbE0A6=7684,rgbE0C6=7685,rgbE0E6=7686,rgbE206=7687,rgbE226=7688,rgbE246=7689,rgbE266=7690,rgbE286=7691,rgbE2A6=7692,rgbE2C6=7693,rgbE2E6=7694,rgbE406=7695,rgbE426=7696,rgbE446=7697,rgbE466=7698,rgbE486=7699,rgbE4A6=7700,rgbE4C6=7701,rgbE4E6=7702,rgbE606=7703,rgbE626=7704,rgbE646=7705,rgbE666=7706,rgbE686=7707,rgbE6A6=7708,rgbE6C6=7709,rgbE6E6=7710,rgbE806=7711,rgbE826=7712,rgbE846=7713,rgbE866=7714,rgbE886=7715,rgbE8A6=7716,rgbE8C6=7717,rgbE8E6=7718,rgbEA06=7719,rgbEA26=7720,rgbEA46=7721,rgbEA66=7722,rgbEA86=7723,rgbEAA6=7724,rgbEAC6=7725,rgbEAE6=7726,rgbEC06=7727,rgbEC26=7728,rgbEC46=7729,rgbEC66=7730,rgbEC86=7731,rgbECA6=7732,rgbECC6=7733,rgbECE6=7734,rgbEE06=7735,rgbEE26=7736,rgbEE46=7737,rgbEE66=7738,rgbEE86=7739,rgbEEA6=7740,rgbEEC6=7741,rgbEEE6=7742,rgb0007=7743,rgb0027=7744,rgb0047=7745,rgb0067=7746,rgb0087=7747,rgb00A7=7748,rgb00C7=7749,rgb00E7=7750,rgb0207=7751,rgb0227=7752,rgb0247=7753,rgb0267=7754,rgb0287=7755,rgb02A7=7756,rgb02C7=7757,rgb02E7=7758,rgb0407=7759,rgb0427=7760,rgb0447=7761,rgb0467=7762,rgb0487=7763,rgb04A7=7764,rgb04C7=7765,rgb04E7=7766,rgb0607=7767,rgb0627=7768,rgb0647=7769,rgb0667=7770,rgb0687=7771,rgb06A7=7772,rgb06C7=7773,rgb06E7=7774,rgb0807=7775,rgb0827=7776,rgb0847=7777,
rgb0867=7778,rgb0887=7779,rgb08A7=7780,rgb08C7=7781,rgb08E7=7782,rgb0A07=7783,rgb0A27=7784,rgb0A47=7785,rgb0A67=7786,rgb0A87=7787,rgb0AA7=7788,rgb0AC7=7789,rgb0AE7=7790,rgb0C07=7791,rgb0C27=7792,rgb0C47=7793,rgb0C67=7794,rgb0C87=7795,rgb0CA7=7796,rgb0CC7=7797,rgb0CE7=7798,rgb0E07=7799,rgb0E27=7800,rgb0E47=7801,rgb0E67=7802,rgb0E87=7803,rgb0EA7=7804,rgb0EC7=7805,rgb0EE7=7806,rgb2007=7807,rgb2027=7808,rgb2047=7809,rgb2067=7810,rgb2087=7811,rgb20A7=7812,rgb20C7=7813,rgb20E7=7814,rgb2207=7815,rgb2227=7816,rgb2247=7817,rgb2267=7818,rgb2287=7819,rgb22A7=7820,rgb22C7=7821,rgb22E7=7822,rgb2407=7823,rgb2427=7824,rgb2447=7825,rgb2467=7826,rgb2487=7827,rgb24A7=7828,rgb24C7=7829,rgb24E7=7830,rgb2607=7831,rgb2627=7832,rgb2647=7833,rgb2667=7834,rgb2687=7835,rgb26A7=7836,rgb26C7=7837,rgb26E7=7838,rgb2807=7839,rgb2827=7840,rgb2847=7841,rgb2867=7842,rgb2887=7843,rgb28A7=7844,rgb28C7=7845,rgb28E7=7846,rgb2A07=7847,rgb2A27=7848,rgb2A47=7849,rgb2A67=7850,rgb2A87=7851,rgb2AA7=7852,rgb2AC7=7853,rgb2AE7=7854,rgb2C07=7855,rgb2C27=7856,rgb2C47=7857,rgb2C67=7858,rgb2C87=7859,rgb2CA7=7860,rgb2CC7=7861,rgb2CE7=7862,rgb2E07=7863,rgb2E27=7864,rgb2E47=7865,rgb2E67=7866,rgb2E87=7867,rgb2EA7=7868,rgb2EC7=7869,rgb2EE7=7870,rgb4007=7871,rgb4027=7872,rgb4047=7873,rgb4067=7874,rgb4087=7875,rgb40A7=7876,rgb40C7=7877,rgb40E7=7878,rgb4207=7879,rgb4227=7880,rgb4247=7881,rgb4267=7882,rgb4287=7883,rgb42A7=7884,rgb42C7=7885,rgb42E7=7886,rgb4407=7887,rgb4427=7888,rgb4447=7889,rgb4467=7890,rgb4487=7891,rgb44A7=7892,rgb44C7=7893,rgb44E7=7894,rgb4607=7895,rgb4627=7896,rgb4647=7897,rgb4667=7898,rgb4687=7899,rgb46A7=7900,rgb46C7=7901,rgb46E7=7902,rgb4807=7903,rgb4827=7904,rgb4847=7905,rgb4867=7906,rgb4887=7907,rgb48A7=7908,rgb48C7=7909,rgb48E7=7910,rgb4A07=7911,rgb4A27=7912,rgb4A47=7913,rgb4A67=7914,rgb4A87=7915,rgb4AA7=7916,rgb4AC7=7917,rgb4AE7=7918,rgb4C07=7919,rgb4C27=7920,rgb4C47=7921,rgb4C67=7922,rgb4C87=7923,rgb4CA7=7924,rgb4CC7=7925,rgb4CE7=7926,rgb4E07=7927,rgb4E27=7928,rgb4E47=7929,rgb4E67=7930,rgb4E87=7931,rgb4EA7=7932,rgb4EC7=7933,rgb4EE7=7934,rgb6007=7935,rgb6027=7936,rgb6047=7937,rgb6067=7938,rgb6087=7939,rgb60A7=7940,rgb60C7=7941,rgb60E7=7942,rgb6207=7943,rgb6227=7944,rgb6247=7945,rgb6267=7946,rgb6287=7947,rgb62A7=7948,rgb62C7=7949,rgb62E7=7950,rgb6407=7951,rgb6427=7952,rgb6447=7953,
rgb6467=7954,rgb6487=7955,rgb64A7=7956,rgb64C7=7957,rgb64E7=7958,rgb6607=7959,rgb6627=7960,rgb6647=7961,rgb6667=7962,rgb6687=7963,rgb66A7=7964,rgb66C7=7965,rgb66E7=7966,rgb6807=7967,rgb6827=7968,rgb6847=7969,rgb6867=7970,rgb6887=7971,rgb68A7=7972,rgb68C7=7973,rgb68E7=7974,rgb6A07=7975,rgb6A27=7976,rgb6A47=7977,rgb6A67=7978,rgb6A87=7979,rgb6AA7=7980,rgb6AC7=7981,rgb6AE7=7982,rgb6C07=7983,rgb6C27=7984,rgb6C47=7985,rgb6C67=7986,rgb6C87=7987,rgb6CA7=7988,rgb6CC7=7989,rgb6CE7=7990,rgb6E07=7991,rgb6E27=7992,rgb6E47=7993,rgb6E67=7994,rgb6E87=7995,rgb6EA7=7996,rgb6EC7=7997,rgb6EE7=7998,rgb8007=7999,rgb8027=8000,rgb8047=8001,rgb8067=8002,rgb8087=8003,rgb80A7=8004,rgb80C7=8005,rgb80E7=8006,rgb8207=8007,rgb8227=8008,rgb8247=8009,rgb8267=8010,rgb8287=8011,rgb82A7=8012,rgb82C7=8013,rgb82E7=8014,rgb8407=8015,rgb8427=8016,rgb8447=8017,rgb8467=8018,rgb8487=8019,rgb84A7=8020,rgb84C7=8021,rgb84E7=8022,rgb8607=8023,rgb8627=8024,rgb8647=8025,rgb8667=8026,rgb8687=8027,rgb86A7=8028,rgb86C7=8029,rgb86E7=8030,rgb8807=8031,rgb8827=8032,rgb8847=8033,rgb8867=8034,rgb8887=8035,rgb88A7=8036,rgb88C7=8037,rgb88E7=8038,rgb8A07=8039,rgb8A27=8040,rgb8A47=8041,rgb8A67=8042,rgb8A87=8043,rgb8AA7=8044,rgb8AC7=8045,rgb8AE7=8046,rgb8C07=8047,rgb8C27=8048,rgb8C47=8049,rgb8C67=8050,rgb8C87=8051,rgb8CA7=8052,rgb8CC7=8053,rgb8CE7=8054,rgb8E07=8055,rgb8E27=8056,rgb8E47=8057,rgb8E67=8058,rgb8E87=8059,rgb8EA7=8060,rgb8EC7=8061,rgb8EE7=8062,rgbA007=8063,rgbA027=8064,rgbA047=8065,rgbA067=8066,rgbA087=8067,rgbA0A7=8068,rgbA0C7=8069,rgbA0E7=8070,rgbA207=8071,rgbA227=8072,rgbA247=8073,rgbA267=8074,rgbA287=8075,rgbA2A7=8076,rgbA2C7=8077,rgbA2E7=8078,rgbA407=8079,rgbA427=8080,rgbA447=8081,rgbA467=8082,rgbA487=8083,rgbA4A7=8084,rgbA4C7=8085,rgbA4E7=8086,rgbA607=8087,rgbA627=8088,rgbA647=8089,rgbA667=8090,rgbA687=8091,rgbA6A7=8092,rgbA6C7=8093,rgbA6E7=8094,rgbA807=8095,rgbA827=8096,rgbA847=8097,rgbA867=8098,rgbA887=8099,rgbA8A7=8100,rgbA8C7=8101,rgbA8E7=8102,rgbAA07=8103,rgbAA27=8104,rgbAA47=8105,rgbAA67=8106,rgbAA87=8107,rgbAAA7=8108,rgbAAC7=8109,rgbAAE7=8110,rgbAC07=8111,rgbAC27=8112,rgbAC47=8113,rgbAC67=8114,rgbAC87=8115,rgbACA7=8116,rgbACC7=8117,rgbACE7=8118,rgbAE07=8119,rgbAE27=8120,rgbAE47=8121,rgbAE67=8122,rgbAE87=8123,rgbAEA7=8124,rgbAEC7=8125,rgbAEE7=8126,rgbC007=8127,rgbC027=8128,rgbC047=8129,
rgbC067=8130,rgbC087=8131,rgbC0A7=8132,rgbC0C7=8133,rgbC0E7=8134,rgbC207=8135,rgbC227=8136,rgbC247=8137,rgbC267=8138,rgbC287=8139,rgbC2A7=8140,rgbC2C7=8141,rgbC2E7=8142,rgbC407=8143,rgbC427=8144,rgbC447=8145,rgbC467=8146,rgbC487=8147,rgbC4A7=8148,rgbC4C7=8149,rgbC4E7=8150,rgbC607=8151,rgbC627=8152,rgbC647=8153,rgbC667=8154,rgbC687=8155,rgbC6A7=8156,rgbC6C7=8157,rgbC6E7=8158,rgbC807=8159,rgbC827=8160,rgbC847=8161,rgbC867=8162,rgbC887=8163,rgbC8A7=8164,rgbC8C7=8165,rgbC8E7=8166,rgbCA07=8167,rgbCA27=8168,rgbCA47=8169,rgbCA67=8170,rgbCA87=8171,rgbCAA7=8172,rgbCAC7=8173,rgbCAE7=8174,rgbCC07=8175,rgbCC27=8176,rgbCC47=8177,rgbCC67=8178,rgbCC87=8179,rgbCCA7=8180,rgbCCC7=8181,rgbCCE7=8182,rgbCE07=8183,rgbCE27=8184,rgbCE47=8185,rgbCE67=8186,rgbCE87=8187,rgbCEA7=8188,rgbCEC7=8189,rgbCEE7=8190,rgbE007=8191,rgbE027=8192,rgbE047=8193,rgbE067=8194,rgbE087=8195,rgbE0A7=8196,rgbE0C7=8197,rgbE0E7=8198,rgbE207=8199,rgbE227=8200,rgbE247=8201,rgbE267=8202,rgbE287=8203,rgbE2A7=8204,rgbE2C7=8205,rgbE2E7=8206,rgbE407=8207,rgbE427=8208,rgbE447=8209,rgbE467=8210,rgbE487=8211,rgbE4A7=8212,rgbE4C7=8213,rgbE4E7=8214,rgbE607=8215,rgbE627=8216,rgbE647=8217,rgbE667=8218,rgbE687=8219,rgbE6A7=8220,rgbE6C7=8221,rgbE6E7=8222,rgbE807=8223,rgbE827=8224,rgbE847=8225,rgbE867=8226,rgbE887=8227,rgbE8A7=8228,rgbE8C7=8229,rgbE8E7=8230,rgbEA07=8231,rgbEA27=8232,rgbEA47=8233,rgbEA67=8234,rgbEA87=8235,rgbEAA7=8236,rgbEAC7=8237,rgbEAE7=8238,rgbEC07=8239,rgbEC27=8240,rgbEC47=8241,rgbEC67=8242,rgbEC87=8243,rgbECA7=8244,rgbECC7=8245,rgbECE7=8246,rgbEE07=8247,rgbEE27=8248,rgbEE47=8249,rgbEE67=8250,rgbEE87=8251,rgbEEA7=8252,rgbEEC7=8253,rgbEEE7=8254,rgb0008=8255,rgb0028=8256,rgb0048=8257,rgb0068=8258,rgb0088=8259,rgb00A8=8260,rgb00C8=8261,rgb00E8=8262,rgb0208=8263,rgb0228=8264,rgb0248=8265,rgb0268=8266,rgb0288=8267,rgb02A8=8268,rgb02C8=8269,rgb02E8=8270,rgb0408=8271,rgb0428=8272,rgb0448=8273,rgb0468=8274,rgb0488=8275,rgb04A8=8276,rgb04C8=8277,
rgb04E8=8278,rgb0608=8279,rgb0628=8280,rgb0648=8281,rgb0668=8282,rgb0688=8283,rgb06A8=8284,rgb06C8=8285,rgb06E8=8286,rgb0808=8287,rgb0828=8288,rgb0848=8289,rgb0868=8290,rgb0888=8291,rgb08A8=8292,rgb08C8=8293,rgb08E8=8294,rgb0A08=8295,rgb0A28=8296,rgb0A48=8297,rgb0A68=8298,rgb0A88=8299,rgb0AA8=8300,rgb0AC8=8301,rgb0AE8=8302,rgb0C08=8303,rgb0C28=8304,rgb0C48=8305,rgb0C68=8306,rgb0C88=8307,rgb0CA8=8308,rgb0CC8=8309,rgb0CE8=8310,rgb0E08=8311,rgb0E28=8312,rgb0E48=8313,rgb0E68=8314,rgb0E88=8315,rgb0EA8=8316,rgb0EC8=8317,rgb0EE8=8318,rgb2008=8319,rgb2028=8320,rgb2048=8321,rgb2068=8322,rgb2088=8323,rgb20A8=8324,rgb20C8=8325,rgb20E8=8326,rgb2208=8327,rgb2228=8328,rgb2248=8329,rgb2268=8330,rgb2288=8331,rgb22A8=8332,rgb22C8=8333,rgb22E8=8334,rgb2408=8335,rgb2428=8336,rgb2448=8337,rgb2468=8338,rgb2488=8339,rgb24A8=8340,rgb24C8=8341,rgb24E8=8342,rgb2608=8343,rgb2628=8344,rgb2648=8345,rgb2668=8346,rgb2688=8347,rgb26A8=8348,rgb26C8=8349,rgb26E8=8350,rgb2808=8351,rgb2828=8352,rgb2848=8353,rgb2868=8354,rgb2888=8355,rgb28A8=8356,rgb28C8=8357,rgb28E8=8358,rgb2A08=8359,rgb2A28=8360,rgb2A48=8361,rgb2A68=8362,rgb2A88=8363,rgb2AA8=8364,rgb2AC8=8365,rgb2AE8=8366,rgb2C08=8367,rgb2C28=8368,rgb2C48=8369,rgb2C68=8370,rgb2C88=8371,rgb2CA8=8372,rgb2CC8=8373,rgb2CE8=8374,rgb2E08=8375,rgb2E28=8376,rgb2E48=8377,rgb2E68=8378,rgb2E88=8379,rgb2EA8=8380,rgb2EC8=8381,rgb2EE8=8382,rgb4008=8383,rgb4028=8384,rgb4048=8385,rgb4068=8386,rgb4088=8387,rgb40A8=8388,rgb40C8=8389,rgb40E8=8390,rgb4208=8391,rgb4228=8392,rgb4248=8393,rgb4268=8394,rgb4288=8395,rgb42A8=8396,rgb42C8=8397,rgb42E8=8398,rgb4408=8399,rgb4428=8400,rgb4448=8401,rgb4468=8402,rgb4488=8403,rgb44A8=8404,rgb44C8=8405,rgb44E8=8406,rgb4608=8407,rgb4628=8408,rgb4648=8409,rgb4668=8410,rgb4688=8411,rgb46A8=8412,rgb46C8=8413,rgb46E8=8414,rgb4808=8415,rgb4828=8416,rgb4848=8417,rgb4868=8418,rgb4888=8419,rgb48A8=8420,rgb48C8=8421,rgb48E8=8422,rgb4A08=8423,rgb4A28=8424,rgb4A48=8425,rgb4A68=8426,rgb4A88=8427,rgb4AA8=8428,rgb4AC8=8429,rgb4AE8=8430,rgb4C08=8431,rgb4C28=8432,rgb4C48=8433,rgb4C68=8434,rgb4C88=8435,rgb4CA8=8436,rgb4CC8=8437,rgb4CE8=8438,rgb4E08=8439,rgb4E28=8440,rgb4E48=8441,rgb4E68=8442,rgb4E88=8443,rgb4EA8=8444,rgb4EC8=8445,rgb4EE8=8446,rgb6008=8447,rgb6028=8448,rgb6048=8449,rgb6068=8450,rgb6088=8451,rgb60A8=8452,rgb60C8=8453,
rgb60E8=8454,rgb6208=8455,rgb6228=8456,rgb6248=8457,rgb6268=8458,rgb6288=8459,rgb62A8=8460,rgb62C8=8461,rgb62E8=8462,rgb6408=8463,rgb6428=8464,rgb6448=8465,rgb6468=8466,rgb6488=8467,rgb64A8=8468,rgb64C8=8469,rgb64E8=8470,rgb6608=8471,rgb6628=8472,rgb6648=8473,rgb6668=8474,rgb6688=8475,rgb66A8=8476,rgb66C8=8477,rgb66E8=8478,rgb6808=8479,rgb6828=8480,rgb6848=8481,rgb6868=8482,rgb6888=8483,rgb68A8=8484,rgb68C8=8485,rgb68E8=8486,rgb6A08=8487,rgb6A28=8488,rgb6A48=8489,rgb6A68=8490,rgb6A88=8491,rgb6AA8=8492,rgb6AC8=8493,rgb6AE8=8494,rgb6C08=8495,rgb6C28=8496,rgb6C48=8497,rgb6C68=8498,rgb6C88=8499,rgb6CA8=8500,rgb6CC8=8501,rgb6CE8=8502,rgb6E08=8503,rgb6E28=8504,rgb6E48=8505,rgb6E68=8506,rgb6E88=8507,rgb6EA8=8508,rgb6EC8=8509,rgb6EE8=8510,rgb8008=8511,rgb8028=8512,rgb8048=8513,rgb8068=8514,rgb8088=8515,rgb80A8=8516,rgb80C8=8517,rgb80E8=8518,rgb8208=8519,rgb8228=8520,rgb8248=8521,rgb8268=8522,rgb8288=8523,rgb82A8=8524,rgb82C8=8525,rgb82E8=8526,rgb8408=8527,rgb8428=8528,rgb8448=8529,rgb8468=8530,rgb8488=8531,rgb84A8=8532,rgb84C8=8533,rgb84E8=8534,rgb8608=8535,rgb8628=8536,rgb8648=8537,rgb8668=8538,rgb8688=8539,rgb86A8=8540,rgb86C8=8541,rgb86E8=8542,rgb8808=8543,rgb8828=8544,rgb8848=8545,rgb8868=8546,rgb8888=8547,rgb88A8=8548,rgb88C8=8549,rgb88E8=8550,rgb8A08=8551,rgb8A28=8552,rgb8A48=8553,rgb8A68=8554,rgb8A88=8555,rgb8AA8=8556,rgb8AC8=8557,rgb8AE8=8558,rgb8C08=8559,rgb8C28=8560,rgb8C48=8561,rgb8C68=8562,rgb8C88=8563,rgb8CA8=8564,rgb8CC8=8565,rgb8CE8=8566,rgb8E08=8567,rgb8E28=8568,rgb8E48=8569,rgb8E68=8570,rgb8E88=8571,rgb8EA8=8572,rgb8EC8=8573,rgb8EE8=8574,rgbA008=8575,rgbA028=8576,rgbA048=8577,rgbA068=8578,rgbA088=8579,rgbA0A8=8580,rgbA0C8=8581,rgbA0E8=8582,rgbA208=8583,rgbA228=8584,rgbA248=8585,rgbA268=8586,rgbA288=8587,rgbA2A8=8588,rgbA2C8=8589,rgbA2E8=8590,rgbA408=8591,rgbA428=8592,rgbA448=8593,rgbA468=8594,rgbA488=8595,rgbA4A8=8596,rgbA4C8=8597,rgbA4E8=8598,rgbA608=8599,rgbA628=8600,rgbA648=8601,rgbA668=8602,rgbA688=8603,rgbA6A8=8604,rgbA6C8=8605,rgbA6E8=8606,rgbA808=8607,rgbA828=8608,rgbA848=8609,rgbA868=8610,rgbA888=8611,rgbA8A8=8612,rgbA8C8=8613,rgbA8E8=8614,rgbAA08=8615,rgbAA28=8616,rgbAA48=8617,rgbAA68=8618,rgbAA88=8619,rgbAAA8=8620,rgbAAC8=8621,rgbAAE8=8622,rgbAC08=8623,rgbAC28=8624,rgbAC48=8625,rgbAC68=8626,rgbAC88=8627,rgbACA8=8628,rgbACC8=8629,
rgbACE8=8630,rgbAE08=8631,rgbAE28=8632,rgbAE48=8633,rgbAE68=8634,rgbAE88=8635,rgbAEA8=8636,rgbAEC8=8637,rgbAEE8=8638,rgbC008=8639,rgbC028=8640,rgbC048=8641,rgbC068=8642,rgbC088=8643,rgbC0A8=8644,rgbC0C8=8645,rgbC0E8=8646,rgbC208=8647,rgbC228=8648,rgbC248=8649,rgbC268=8650,rgbC288=8651,rgbC2A8=8652,rgbC2C8=8653,rgbC2E8=8654,rgbC408=8655,rgbC428=8656,rgbC448=8657,rgbC468=8658,rgbC488=8659,rgbC4A8=8660,rgbC4C8=8661,rgbC4E8=8662,rgbC608=8663,rgbC628=8664,rgbC648=8665,rgbC668=8666,rgbC688=8667,rgbC6A8=8668,rgbC6C8=8669,rgbC6E8=8670,rgbC808=8671,rgbC828=8672,rgbC848=8673,rgbC868=8674,rgbC888=8675,rgbC8A8=8676,rgbC8C8=8677,rgbC8E8=8678,rgbCA08=8679,rgbCA28=8680,rgbCA48=8681,rgbCA68=8682,rgbCA88=8683,rgbCAA8=8684,rgbCAC8=8685,rgbCAE8=8686,rgbCC08=8687,rgbCC28=8688,rgbCC48=8689,rgbCC68=8690,rgbCC88=8691,rgbCCA8=8692,rgbCCC8=8693,rgbCCE8=8694,rgbCE08=8695,rgbCE28=8696,rgbCE48=8697,rgbCE68=8698,rgbCE88=8699,rgbCEA8=8700,rgbCEC8=8701,rgbCEE8=8702,rgbE008=8703,rgbE028=8704,rgbE048=8705,rgbE068=8706,rgbE088=8707,rgbE0A8=8708,rgbE0C8=8709,rgbE0E8=8710,rgbE208=8711,rgbE228=8712,rgbE248=8713,rgbE268=8714,rgbE288=8715,rgbE2A8=8716,rgbE2C8=8717,rgbE2E8=8718,rgbE408=8719,rgbE428=8720,rgbE448=8721,rgbE468=8722,rgbE488=8723,rgbE4A8=8724,rgbE4C8=8725,rgbE4E8=8726,rgbE608=8727,rgbE628=8728,rgbE648=8729,rgbE668=8730,rgbE688=8731,rgbE6A8=8732,rgbE6C8=8733,rgbE6E8=8734,rgbE808=8735,rgbE828=8736,rgbE848=8737,rgbE868=8738,rgbE888=8739,rgbE8A8=8740,rgbE8C8=8741,rgbE8E8=8742,rgbEA08=8743,rgbEA28=8744,rgbEA48=8745,rgbEA68=8746,rgbEA88=8747,rgbEAA8=8748,rgbEAC8=8749,rgbEAE8=8750,rgbEC08=8751,rgbEC28=8752,rgbEC48=8753,rgbEC68=8754,rgbEC88=8755,rgbECA8=8756,rgbECC8=8757,rgbECE8=8758,rgbEE08=8759,rgbEE28=8760,rgbEE48=8761,rgbEE68=8762,rgbEE88=8763,rgbEEA8=8764,rgbEEC8=8765,rgbEEE8=8766,rgb0009=8767,rgb0029=8768,rgb0049=8769,rgb0069=8770,rgb0089=8771,rgb00A9=8772,rgb00C9=8773,rgb00E9=8774,rgb0209=8775,rgb0229=8776,rgb0249=8777,rgb0269=8778,rgb0289=8779,rgb02A9=8780,rgb02C9=8781,rgb02E9=8782,rgb0409=8783,rgb0429=8784,rgb0449=8785,rgb0469=8786,rgb0489=8787,rgb04A9=8788,rgb04C9=8789,rgb04E9=8790,rgb0609=8791,rgb0629=8792,rgb0649=8793,rgb0669=8794,rgb0689=8795,rgb06A9=8796,rgb06C9=8797,rgb06E9=8798,rgb0809=8799,rgb0829=8800,rgb0849=8801,rgb0869=8802,rgb0889=8803,rgb08A9=8804,rgb08C9=8805,
rgb08E9=8806,rgb0A09=8807,rgb0A29=8808,rgb0A49=8809,rgb0A69=8810,rgb0A89=8811,rgb0AA9=8812,rgb0AC9=8813,rgb0AE9=8814,rgb0C09=8815,rgb0C29=8816,rgb0C49=8817,rgb0C69=8818,rgb0C89=8819,rgb0CA9=8820,rgb0CC9=8821,rgb0CE9=8822,rgb0E09=8823,rgb0E29=8824,rgb0E49=8825,rgb0E69=8826,rgb0E89=8827,rgb0EA9=8828,rgb0EC9=8829,rgb0EE9=8830,rgb2009=8831,rgb2029=8832,rgb2049=8833,rgb2069=8834,rgb2089=8835,rgb20A9=8836,rgb20C9=8837,rgb20E9=8838,rgb2209=8839,rgb2229=8840,rgb2249=8841,rgb2269=8842,rgb2289=8843,rgb22A9=8844,rgb22C9=8845,rgb22E9=8846,rgb2409=8847,rgb2429=8848,rgb2449=8849,rgb2469=8850,rgb2489=8851,rgb24A9=8852,rgb24C9=8853,rgb24E9=8854,rgb2609=8855,rgb2629=8856,rgb2649=8857,rgb2669=8858,rgb2689=8859,rgb26A9=8860,rgb26C9=8861,rgb26E9=8862,rgb2809=8863,rgb2829=8864,rgb2849=8865,rgb2869=8866,rgb2889=8867,rgb28A9=8868,rgb28C9=8869,rgb28E9=8870,rgb2A09=8871,rgb2A29=8872,rgb2A49=8873,rgb2A69=8874,rgb2A89=8875,rgb2AA9=8876,rgb2AC9=8877,rgb2AE9=8878,rgb2C09=8879,rgb2C29=8880,rgb2C49=8881,rgb2C69=8882,rgb2C89=8883,rgb2CA9=8884,rgb2CC9=8885,rgb2CE9=8886,rgb2E09=8887,rgb2E29=8888,rgb2E49=8889,rgb2E69=8890,rgb2E89=8891,rgb2EA9=8892,rgb2EC9=8893,rgb2EE9=8894,rgb4009=8895,rgb4029=8896,rgb4049=8897,rgb4069=8898,rgb4089=8899,rgb40A9=8900,rgb40C9=8901,rgb40E9=8902,rgb4209=8903,rgb4229=8904,rgb4249=8905,rgb4269=8906,rgb4289=8907,rgb42A9=8908,rgb42C9=8909,rgb42E9=8910,rgb4409=8911,rgb4429=8912,rgb4449=8913,rgb4469=8914,rgb4489=8915,rgb44A9=8916,rgb44C9=8917,rgb44E9=8918,rgb4609=8919,rgb4629=8920,rgb4649=8921,rgb4669=8922,rgb4689=8923,rgb46A9=8924,rgb46C9=8925,rgb46E9=8926,rgb4809=8927,rgb4829=8928,rgb4849=8929,rgb4869=8930,rgb4889=8931,rgb48A9=8932,rgb48C9=8933,rgb48E9=8934,rgb4A09=8935,rgb4A29=8936,rgb4A49=8937,rgb4A69=8938,rgb4A89=8939,rgb4AA9=8940,rgb4AC9=8941,rgb4AE9=8942,rgb4C09=8943,rgb4C29=8944,rgb4C49=8945,rgb4C69=8946,rgb4C89=8947,rgb4CA9=8948,rgb4CC9=8949,rgb4CE9=8950,rgb4E09=8951,rgb4E29=8952,rgb4E49=8953,
rgb4E69=8954,rgb4E89=8955,rgb4EA9=8956,rgb4EC9=8957,rgb4EE9=8958,rgb6009=8959,rgb6029=8960,rgb6049=8961,rgb6069=8962,rgb6089=8963,rgb60A9=8964,rgb60C9=8965,rgb60E9=8966,rgb6209=8967,rgb6229=8968,rgb6249=8969,rgb6269=8970,rgb6289=8971,rgb62A9=8972,rgb62C9=8973,rgb62E9=8974,rgb6409=8975,rgb6429=8976,rgb6449=8977,rgb6469=8978,rgb6489=8979,rgb64A9=8980,rgb64C9=8981,rgb64E9=8982,rgb6609=8983,rgb6629=8984,rgb6649=8985,rgb6669=8986,rgb6689=8987,rgb66A9=8988,rgb66C9=8989,rgb66E9=8990,rgb6809=8991,rgb6829=8992,rgb6849=8993,rgb6869=8994,rgb6889=8995,rgb68A9=8996,rgb68C9=8997,rgb68E9=8998,rgb6A09=8999,rgb6A29=9000,rgb6A49=9001,rgb6A69=9002,rgb6A89=9003,rgb6AA9=9004,rgb6AC9=9005,rgb6AE9=9006,rgb6C09=9007,rgb6C29=9008,rgb6C49=9009,rgb6C69=9010,rgb6C89=9011,rgb6CA9=9012,rgb6CC9=9013,rgb6CE9=9014,rgb6E09=9015,rgb6E29=9016,rgb6E49=9017,rgb6E69=9018,rgb6E89=9019,rgb6EA9=9020,rgb6EC9=9021,rgb6EE9=9022,rgb8009=9023,rgb8029=9024,rgb8049=9025,rgb8069=9026,rgb8089=9027,rgb80A9=9028,rgb80C9=9029,rgb80E9=9030,rgb8209=9031,rgb8229=9032,rgb8249=9033,rgb8269=9034,rgb8289=9035,rgb82A9=9036,rgb82C9=9037,rgb82E9=9038,rgb8409=9039,rgb8429=9040,rgb8449=9041,rgb8469=9042,rgb8489=9043,rgb84A9=9044,rgb84C9=9045,rgb84E9=9046,rgb8609=9047,rgb8629=9048,rgb8649=9049,rgb8669=9050,rgb8689=9051,rgb86A9=9052,rgb86C9=9053,rgb86E9=9054,rgb8809=9055,rgb8829=9056,rgb8849=9057,rgb8869=9058,rgb8889=9059,rgb88A9=9060,rgb88C9=9061,rgb88E9=9062,rgb8A09=9063,rgb8A29=9064,rgb8A49=9065,rgb8A69=9066,rgb8A89=9067,rgb8AA9=9068,rgb8AC9=9069,rgb8AE9=9070,rgb8C09=9071,rgb8C29=9072,rgb8C49=9073,rgb8C69=9074,rgb8C89=9075,rgb8CA9=9076,rgb8CC9=9077,rgb8CE9=9078,rgb8E09=9079,rgb8E29=9080,rgb8E49=9081,rgb8E69=9082,rgb8E89=9083,rgb8EA9=9084,rgb8EC9=9085,rgb8EE9=9086,rgbA009=9087,rgbA029=9088,rgbA049=9089,rgbA069=9090,rgbA089=9091,rgbA0A9=9092,rgbA0C9=9093,rgbA0E9=9094,rgbA209=9095,rgbA229=9096,rgbA249=9097,rgbA269=9098,rgbA289=9099,rgbA2A9=9100,rgbA2C9=9101,rgbA2E9=9102,rgbA409=9103,rgbA429=9104,rgbA449=9105,rgbA469=9106,rgbA489=9107,rgbA4A9=9108,rgbA4C9=9109,rgbA4E9=9110,rgbA609=9111,rgbA629=9112,rgbA649=9113,rgbA669=9114,rgbA689=9115,rgbA6A9=9116,rgbA6C9=9117,rgbA6E9=9118,rgbA809=9119,rgbA829=9120,rgbA849=9121,rgbA869=9122,rgbA889=9123,rgbA8A9=9124,rgbA8C9=9125,rgbA8E9=9126,rgbAA09=9127,rgbAA29=9128,rgbAA49=9129,
rgbAA69=9130,rgbAA89=9131,rgbAAA9=9132,rgbAAC9=9133,rgbAAE9=9134,rgbAC09=9135,rgbAC29=9136,rgbAC49=9137,rgbAC69=9138,rgbAC89=9139,rgbACA9=9140,rgbACC9=9141,rgbACE9=9142,rgbAE09=9143,rgbAE29=9144,rgbAE49=9145,rgbAE69=9146,rgbAE89=9147,rgbAEA9=9148,rgbAEC9=9149,rgbAEE9=9150,rgbC009=9151,rgbC029=9152,rgbC049=9153,rgbC069=9154,rgbC089=9155,rgbC0A9=9156,rgbC0C9=9157,rgbC0E9=9158,rgbC209=9159,rgbC229=9160,rgbC249=9161,rgbC269=9162,rgbC289=9163,rgbC2A9=9164,rgbC2C9=9165,rgbC2E9=9166,rgbC409=9167,rgbC429=9168,rgbC449=9169,rgbC469=9170,rgbC489=9171,rgbC4A9=9172,rgbC4C9=9173,rgbC4E9=9174,rgbC609=9175,rgbC629=9176,rgbC649=9177,rgbC669=9178,rgbC689=9179,rgbC6A9=9180,rgbC6C9=9181,rgbC6E9=9182,rgbC809=9183,rgbC829=9184,rgbC849=9185,rgbC869=9186,rgbC889=9187,rgbC8A9=9188,rgbC8C9=9189,rgbC8E9=9190,rgbCA09=9191,rgbCA29=9192,rgbCA49=9193,rgbCA69=9194,rgbCA89=9195,rgbCAA9=9196,rgbCAC9=9197,rgbCAE9=9198,rgbCC09=9199,rgbCC29=9200,rgbCC49=9201,rgbCC69=9202,rgbCC89=9203,rgbCCA9=9204,rgbCCC9=9205,rgbCCE9=9206,rgbCE09=9207,rgbCE29=9208,rgbCE49=9209,rgbCE69=9210,rgbCE89=9211,rgbCEA9=9212,rgbCEC9=9213,rgbCEE9=9214,rgbE009=9215,rgbE029=9216,rgbE049=9217,rgbE069=9218,rgbE089=9219,rgbE0A9=9220,rgbE0C9=9221,rgbE0E9=9222,rgbE209=9223,rgbE229=9224,rgbE249=9225,rgbE269=9226,rgbE289=9227,rgbE2A9=9228,rgbE2C9=9229,rgbE2E9=9230,rgbE409=9231,rgbE429=9232,rgbE449=9233,rgbE469=9234,rgbE489=9235,rgbE4A9=9236,rgbE4C9=9237,rgbE4E9=9238,rgbE609=9239,rgbE629=9240,rgbE649=9241,rgbE669=9242,rgbE689=9243,rgbE6A9=9244,rgbE6C9=9245,rgbE6E9=9246,rgbE809=9247,rgbE829=9248,rgbE849=9249,rgbE869=9250,rgbE889=9251,rgbE8A9=9252,rgbE8C9=9253,rgbE8E9=9254,rgbEA09=9255,rgbEA29=9256,rgbEA49=9257,rgbEA69=9258,rgbEA89=9259,rgbEAA9=9260,rgbEAC9=9261,rgbEAE9=9262,rgbEC09=9263,rgbEC29=9264,rgbEC49=9265,rgbEC69=9266,rgbEC89=9267,rgbECA9=9268,rgbECC9=9269,rgbECE9=9270,rgbEE09=9271,rgbEE29=9272,rgbEE49=9273,rgbEE69=9274,rgbEE89=9275,rgbEEA9=9276,rgbEEC9=9277,rgbEEE9=9278
};

extern char *color_keywords[TOTAL_COLORS_AND_GLASS];

#define TOTAL_BRIGHTS 32

struct Crayon {
public:
 float floats[4];
 double rd, gd, bd, ad;
 float rf, gf, bf, af;
 int r, g, b, a;
 bool f255;
 void fromString(const char *s) {
  if ( *s == '\0' ) { Pick(black); return; }
  const char *p=s;
  std::string key;
  SKEYPAIR;
  const char *q=key.c_str();  
  if ( *q == '#' ) {
   CSS(q);
  } else if ( is_decimal(q) ) {
   rf=(float)atof(key.c_str()); if ( rf > 1.0f ) { r=(int) rf; rf=rf/255.0f; }
   SPARAM(gf,float,atof); if ( gf > 1.0f ) { g=(int) gf; gf=gf/255.0f; }
   SPARAM(bf,float,atof); if ( bf > 1.0f ) { b=(int) bf; bf=bf/255.0f; }
   SPARAM(af,float,atof); if ( af > 1.0f ) { a=(int) af; af=af/255.0f; }
   Float(rf,gf,bf,af);
  } else Named((char *)key.c_str());
 }
 void fromStringFloat(const char *s) {
  if ( *s == '\0' ) { Pick(black); return; }
  const char *p=s;
  std::string key;
  SPARAM(rf,float,atof); if ( gf > 1.0f ) { g=(int) gf; gf=gf/255.0f; }
  SPARAM(gf,float,atof); if ( gf > 1.0f ) { g=(int) gf; gf=gf/255.0f; }
  SPARAM(bf,float,atof); if ( bf > 1.0f ) { b=(int) bf; bf=bf/255.0f; }
  SPARAM(af,float,atof); if ( af > 1.0f ) { a=(int) af; af=af/255.0f; }
  Float(rf,gf,bf,af);
 }
 void fromStringInt(const char *s) {
  if ( *s == '\0' ) { Pick(black); return; }
  const char *p=s;
  std::string key;
  SPARAM(r,int,atoi); if ( gf > 1.0f ) { g=(int) gf; gf=gf/255.0f; }
  SPARAM(g,int,atoi); if ( gf > 1.0f ) { g=(int) gf; gf=gf/255.0f; }
  SPARAM(b,int,atoi); if ( bf > 1.0f ) { b=(int) bf; bf=bf/255.0f; }
  SPARAM(a,int,atoi); if ( af > 1.0f ) { a=(int) af; af=af/255.0f; }
  Int(r,g,b,a);
 }
 string toString() {
  static char buf[128];
  FORMAT(buf,128,"%1.5f %1.5f %1.5f %1.5f", (float) floats[0], (float) floats[1], (float) floats[2], (float) floats[3]);
  return string(buf);
 }
 string toStringInts() {
  static char buf[128];
  FORMAT(buf,128,"%d %d %d %d", r,g,b,a);
  return string(buf);
 }
 void BinaryWrite(BinaryFile *open);
 void BinaryRead(BinaryFile *open);

	Crayon(void);
 Crayon(Colors c) { Pick(c); }
// Crayon(Crayon c) { fromCrayon(c); } // can't do this one
	Crayon(int R, int G, int B);
	Crayon(float R, float G, float B);
	Crayon(float R, float G, float B, float A);
	~Crayon(void);
 void Into( float *out4 ) {
  out4[0]=rf;
  out4[1]=gf;
  out4[2]=bf;
  out4[3]=af;
 }
 Crayon Anti() {
  Crayon c; c=(*this);
  c.Double(1.0-c.rd,1.0-c.gd,1.0-c.bd,c.ad);
  return c;
 }
 string toCSS(){
  return string("#")+to_hex(r)+to_hex(g)+to_hex(b)+to_hex(a);  
 }
 void CSS( const char *s ) {
  char piece[3];
  piece[2]='\0';
  if ( *s=='#' ) s++;
  int chars=(int) strlen(s);
  if ( chars == 8 ) { // #RRGGBBAA
   piece[0]=s[0];
   piece[1]=s[1];
   r=from_hex(piece);
   piece[0]=s[2];
   piece[1]=s[3];
   g=from_hex(piece);
   piece[0]=s[4];
   piece[1]=s[5];
   b=from_hex(piece);
   piece[0]=s[6];
   piece[1]=s[7];
   a=from_hex(piece);
  } else if ( chars == 6 ) { // ##RRGGBB
   piece[0]=s[0];
   piece[1]=s[1];
   r=from_hex(piece);
   piece[0]=s[2];
   piece[1]=s[3];
   g=from_hex(piece);
   piece[0]=s[4];
   piece[1]=s[5];
   b=from_hex(piece);
   a=255;
  } else if ( chars == 4 ) { // #RGBA
   piece[1]='\0';
   piece[0]=s[0];
   r=16*from_hex(piece);
   piece[0]=s[1];
   g=16*from_hex(piece);
   piece[0]=s[2];
   b=16*from_hex(piece);
   a=255;
  } else if ( chars == 3 ) { // #RGB
   piece[1]='\0';
   piece[0]=s[0];
   r=16*from_hex(piece);
   piece[0]=s[1];
   g=16*from_hex(piece);
   piece[0]=s[2];
   b=16*from_hex(piece);
   piece[0]=s[2];
   a=16*from_hex(piece);
  } else if ( chars == 2 ) { // #LL  luminosity (greyscale)
   piece[0]=s[0];
   piece[1]=s[1];
   r=g=b=from_hex(piece); a=255;
  }
  Int(r,g,b,a);
 }

 void gl();
 void gl(Blends blend);

 void Named( const char *n ) {
  while ( *n == ' ' ) n++;
  if ( strlen(n) == 0 ) {
   OUTPUT("Crayon:Named(`%s`) Empty Keyword Provided, No Effect to Setting\n\r",n);
   return;
  }
  if ( !str_cmp(n,"any") ) { Any(); return; }
  if ( *n == '#' ) { CSS(n); return; }
  for ( int i=0; i<TOTAL_COLORS_AND_GLASS-1; i++ ) if ( !str_prefix(n,color_keywords[i]) ) { Pick((Colors)i); return; }
  for ( int i=0; i<TOTAL_COLORS_AND_GLASS-1; i++ ) if ( !str_infix(n,color_keywords[i]) ) { Pick((Colors)i); return; }
  // split camelize, strip numbers, try prefix/infix again (not yet implemented)
  // otherwise...
  OUTPUT("Crayon::Named(`%s`) reports unidentified color keyword, ignoring; value remains set to (%1.2f %1.2f %1.2f %1.2f).\n", n, rf, gf, bf, af );
 }

 void Red( int i )   { r=i; rf=(float) i/255.0f; floats[0]=rf; }
 void Green( int i ) { g=i; gf=(float) i/255.0f; floats[1]=gf; }
 void Blue( int i )  { b=i; bf=(float) i/255.0f; floats[2]=bf; }

 void Redf( float i )   { r=(int) (i*255.0f); rf=i; floats[0]=rf; }
 void Greenf( float i ) { g=(int) (i*255.0f); gf=i; floats[1]=gf; }
 void Bluef( float i )  { b=(int) (i*255.0f); bf=i; floats[2]=bf; }

 void Redf255( float i )   { r=(int) (i); rf=i/255.0f; floats[0]=rf; }
 void Greenf255( float i ) { g=(int) (i); gf=i/255.0f; floats[1]=gf; }
 void Bluef255( float i )  { b=(int) (i); bf=i/255.0f; floats[2]=bf; }

 void Smear( double amount=0.1 ) {
  Double(
   UMIN(1.0,rd+(0.5-uniform())*amount),
   UMIN(1.0,gd+(0.5-uniform())*amount),
   UMIN(1.0,bd+(0.5-uniform())*amount),
   ad
  );
 }
 void Smear( having alphaSmear, double amount=0.1 ) {
  Double(
   UMIN(1.0,rd+(0.5-uniform())*amount),
   UMIN(1.0,gd+(0.5-uniform())*amount),
   UMIN(1.0,bd+(0.5-uniform())*amount),
   UMIN(1.0,ad+uniform()*amount)
  );
 }

 void Wrapi(void);
 void SubWrapf( float sub );
 void Absi(void);
 void ColorInterp( Crayon A, Crayon B, float f );
 void ColorInterp( Crayon A, Crayon B, double d ) { ColorInterp(A,B,(float)d); }
 void ColorCombine4( Crayon A, Crayon B, Crayon C, Crayon D, float i1, float i2, float i3, float alpha );
 void ColorCombine4( Crayon A, Crayon B, Crayon C, Crayon D, double i1, double i2, double i3, double alpha ) {
  ColorCombine4(A,B,C,D,(float)i1,(float)i2,(float)i3,(float)alpha); 
 }
 void ColorAddTransparency( Crayon A, Crayon B );

 void SubColorFactori( Crayon A, Crayon B, float f );
 void SubColorFactori( Crayon A, Crayon B, double d ) { SubColorFactori(A,B,(float)d); }

 void SubColori( Crayon A, Crayon B );
 void AddColori( Crayon A, Crayon B );

 void SubColorf( Crayon A, Crayon B );
 void AddColorf( Crayon A, Crayon B );

 Crayon ScaleByAlpha( float alpha );
 Crayon ScaleByAlpha( double alpha );
 void ScaleByAlphaf( float alpha ); // legacy
 void ScaleByAlphaf( double alpha ) { ScaleByAlphaf((float)alpha); }

 // Increases the color component by half the distance between its current setting and 1
 void AmplifyComponents() {
  float Rremainder=(1.0f-rf)/2.0f;
  float Gremainder=(1.0f-gf)/2.0f;
  float Bremainder=(1.0f-bf)/2.0f;
  Float(rf+Rremainder,gf+Gremainder,bf+Bremainder,af);
 }

 Crayon Alpha( float alpha );
 Crayon Alpha( double alpha );

 void Int( int R, int G, int B, int A );
 void Float( float R, float G, float B, float A );
 void Float255( float R, float G, float B, float A );
 void Double( double R, double G, double B, double A ) { rd=R; gd=G; bd=B; ad=A; Float( (float) R, (float) G, (float) B, (float) A ); }
 void Double255( double R, double G, double B, double A ) { rd=R/255.0; gd=G/255.0; bd=B/255.0; ad=A/255.0; Float( (float) rd, (float) gd, (float) bd, (float) ad ); }
 void Name( const char *s );

 const char *Parse( const char *line, int values ) {
   char c[1024];
   const char *p=line;
   p=one_argument(p,c); rf = (float) atof(c);
   if ( values > 1 ) { p=one_argument(p,c); gf = (float) atof(c); } else gf=1.0f;
   if ( values > 2 ) { p=one_argument(p,c); bf = (float) atof(c); } else bf=1.0f;
   if ( values > 3 ) { p=one_argument(p,c); af = (float) atof(c); } else af=1.0f;
   return p;
 }

 void Opacity( int o );
 void Opacity( float o );

 void fromCrayon( Crayon C );
 void fromFloat( float *f );
 void fromFloats( float *f );

 // These five functions allocate memory and return it
 float *asFloatPrenormalizedf();
 float *asFloati();
 float *asFloatf();
 float *asFloatAlphai( float alpha );
 float *asFloatAlphaf( float alpha );

 void Pick( Colors c );
 void Any();
 void Any( int seed );
 void random();
 void random( int seed );
 Crayon fuzz( float l, float h ) {
  Crayon fizz;
  fizz.Float(
   float_range( this->rf*l, this->rf*h ),
   float_range( this->gf*l, this->gf*h ),
   float_range( this->bf*l, this->bf*h ),
   this->af
   );
  return fizz;
 }
 void fizz( float l, float h ) {
  Crayon fuzz;
  Float(
   float_range( this->rf*l, this->rf*h ),
   float_range( this->gf*l, this->gf*h ),
   float_range( this->bf*l, this->bf*h ),
   this->af
   );
 }
 // Multiplies stored by input
 void Multiply( Crayon a ) { Float( rf*a.rf, gf*a.gf, bf*a.bf, af ); }
 // Stores multiplication, multiply alpha factors?
 void Multiply( Crayon a, Crayon b ) { Float( a.rf*b.rf, a.gf*b.gf, a.bf*b.bf, a.af ); }
 // Stores multiplication, multiply alpha factors?
 void Multiply( Crayon a, Crayon b, bool alpha ) { Float( a.rf*b.rf, a.gf*b.gf, a.bf*b.bf, alpha ? a.af*b.af : a.af ); }
 void MultiplyClampRGB( float f ) { MultiplyClampRGB((double)f); }
 void MultiplyClampRGB( double f ) {
  Double( rd*f, gd*f, bd*f, ad );
  if ( rd > 1.0 ) rd=1.0;
  if ( gd > 1.0 ) gd=1.0;
  if ( bd > 1.0 ) bd=1.0;
 }
 void MultiplyClampRGB( float fr, float fg, float fb ) { MultiplyClampRGB((double)fr,(double)fg,(double)fb); }
 void MultiplyClampRGB( double fr, double fg, double fb ) {
  Double( rd*fr, gd*fg, bd*fb, ad );
  if ( rd > 1.0 ) rd=1.0;
  if ( gd > 1.0 ) gd=1.0;
  if ( bd > 1.0 ) bd=1.0;
  if ( rd < 1.0 ) rd=0.0;
  if ( gd < 1.0 ) gd=0.0;
  if ( bd < 1.0 ) bd=0.0;
  Double( rd, gd, bd, ad );
 }
 void MultiplyWrapRGB( double f ) {
  Double( rd*f, gd*f, bd*f, ad );
  while ( rd > 1.0 ) rd-=1.0;
  while ( gd > 1.0 ) gd-=1.0;
  while ( bd > 1.0 ) bd-=1.0;
  while ( rd < 0.0 ) rd+=1.0;
  while ( gd < 0.0 ) gd+=1.0;
  while ( bd < 0.0 ) bd+=1.0;
  Double( rd, gd, bd, ad );
 }
 void MultiplyWrapRGB( float f ) { MultiplyWrapRGB((double)f); }
 void MultiplyWrapRGB( float fr, float fg, float fb ) { MultiplyWrapRGB((double)fr,(double)fg,(double)fb); }
 void MultiplyWrapRGB( double fr, double fg, double fb ) {
  Double( rd*fr, gd*fg, bd*fb, ad );
  while ( rd > 1.0 ) rd-=1.0;
  while ( gd > 1.0 ) gd-=1.0;
  while ( bd > 1.0 ) bd-=1.0;
  while ( rd < 0.0 ) rd+=1.0;
  while ( gd < 0.0 ) gd+=1.0;
  while ( bd < 0.0 ) bd+=1.0;
  Double( rd, gd, bd, ad );
 }
 int Near( Crayon C ) {
  float NR,NG,NB;
  NR=rf-C.rf;
  NG=gf-C.gf;
  NB=bf-C.bf;
  return abs((int) ((NR+NG+NB)/3.0f));
 }
 bool Near( Crayon C, int nearness ) {
  return 
     ( abs((int)((rf-C.rf)*255.0f)) < nearness
    && abs((int)((gf-C.gf)*255.0f)) < nearness
    && abs((int)((bf-C.bf)*255.0f)) < nearness );
 }
 void Multiply( float f ) {
  Float(f*rf,f*gf,f*bf,f*af);
 }
 void Divide( float f ) {
  if ( f == 0.0f ) OUTPUT("Crayon::Divide(%f) warns: divide by zero\n", (float) f );
  else
  Float(rf/f,gf/f,bf/f,af/f);
 }
 bool Equals( float rf, float gf, float bf, float af ) {
  return this->rf == rf && this->gf == gf && this->bf == bf && this->af == af;
 }

 bool Equals( int r, int g, int b, int a ) {
  return this->r == r && this->g == g && this->b == b && this->a == a;
 }
 
 bool Equals( float *f ) {
  return floats[0]==f[0] && floats[1]==f[1] && floats[2]==f[2] && floats[3]==f[3];
 }

 bool Equals( Crayon *c ) {
  return floats[0]==c->floats[0] && floats[1]==c->floats[1] && floats[2]==c->floats[2] && floats[3]==c->floats[3];
 }
 void Lerp( float t, Crayon A, Crayon B ) {
  Float( LERP1(A.rf,B.rf,t), LERP1(A.gf,B.gf,t), LERP1(A.bf,B.bf,t), LERP1(A.af,B.af,t) );
 }
 void Lerp(double t, Crayon A, Crayon B) {
  Double(LERP1(A.r, B.r, t), LERP1(A.g, B.g, t), LERP1(A.b, B.b, t), LERP1(A.a, B.a, t));
 }
 void Mix( float t, Crayon B ) { Lerp( t, *this, B ); }
 void Mixed( float t, Crayon A, Crayon B ) { Lerp( t, A, B ); }
 void Solid( int i  );
 void Brights( int i );
 void Vivids( int i );
 float Luminance() { return (rf+gf+bf)/3.0f; }
 float LuminanceWeighted() { return sqrt( 0.241f * SQ(rf) + 0.691f * SQ(gf) + 0.068f * SQ(bf) ); }
 Crayon& operator= ( const Crayon& c ) { fromCrayon(c); return *this; }
 Crayon& operator= ( Colors c ) { Pick(c); return *this; }
 Crayon& operator= ( int i ) { Solid(i); return *this; }
 Crayon& operator-= ( const Crayon &c ) { SubColorf(*this,c); return *this; }
 Crayon& operator+= ( const Crayon &c ) { AddColorf(*this,c); return *this; }
 Crayon& operator*= ( const Crayon &c ) { Multiply(*this,c); return *this; }
 Crayon& operator*= ( float f ) { Multiply(f); return *this; }
 Crayon& operator*= ( double d ) { Multiply((float)d); return *this; }
 Crayon& operator/= ( float f ) { Divide(f); return *this; }
 Crayon& operator/= ( double d ) { Divide((float)d); return *this; }
 // X11 Colorname Presets
 void x_AliceBlue()/* X11: F0F8FF */{ Float( 0.94f,0.97f,1.0f, 1.0f); }        void x_LightSeaGreen()/* X11: 20B2AA */{ Float( 0.13f,0.70f,0.67f, 1.0f); }
 void x_AntiqueWhite()/* X11: FAEBD7 */{ Float( 0.98f,0.92f,0.84f, 1.0f); }      void x_LightSkyBlue()/* X11: 87CEFA */{ Float( 0.53f,0.81f,0.98f, 1.0f); }
 void x_Aqua()/* X11: 00FFFF */{ Float( 0.0f,1.0f,1.0f, 1.0f); }                void x_LightSlateGray()/* X11: 778899 */{ Float( 0.47f,0.53f,0.60f, 1.0f); }
 void x_Aquamarine()/* X11: 7FFFD4 */{ Float( 0.50f,1.0f,0.83f, 1.0f); }         void x_LightSlateGrey()/* X11: 778899 */{ Float( 0.47f,0.53f,0.60f, 1.0f); }
 void x_Azure()/* X11: F0FFFF */{ Float( 0.94f,1.0f,1.0f, 1.0f); }              void x_LightSteelBlue()/* X11: B0C4DE */{ Float( 0.69f,0.77f,0.87f, 1.0f); }
 void x_Beige()/* X11: F5F5DC */{ Float( 0.96f,0.96f,0.86f, 1.0f); }             void x_LightYellow()/* X11: FFFFE0 */{ Float( 1.0f,1.0f,0.88f, 1.0f); }
 void x_Bisque()/* X11: FFE4C4 */{ Float( 1.0f,0.89f,0.77f, 1.0f); }             void x_Lime()/* X11: 00FF00 */{ Float( 0.0f,1.0f,0.0f, 1.0f); }
 void x_Black()/* X11: 000000 */{ Float( 0.0f,0.0f,0.0f, 1.0f); }                void x_LimeGreen()/* X11: 32CD32 */{ Float( 0.20f,0.80f,0.20f, 1.0f); }
 void x_BlanchedAlmond()/* X11: FFEBCD */{ Float( 1.0f,0.92f,0.80f, 1.0f); }     void x_Linen()/* X11: FAF0E6 */{ Float( 0.98f,0.94f,0.90f, 1.0f); }
 void x_Blue()/* X11: 0000FF */{ Float( 0.0f,0.0f,1.0f, 1.0f); }                void x_Magenta()/* X11: FF00FF */{ Float( 1.0f,0.0f,1.0f, 1.0f); }
 void x_BlueViolet()/* X11: 8A2BE2 */{ Float( 0.54f,0.17f,0.89f, 1.0f); }        void x_Fuchsia()/* X11: FF00FF */{ Float( 1.0f,0.0f,1.0f, 1.0f); }
 void x_Brown()/* X11: A52A2A */{ Float( 0.65f,0.16f,0.16f, 1.0f); }             void x_Maroon()/* X11: B03060 */{ Float( 0.69f,0.19f,0.38f, 1.0f); }
 void x_Burlywood()/* X11: DEB887 */{ Float( 0.87f,0.72f,0.53f, 1.0f); }         void x_WebMaroon()/* X11: 7F0000 */{ Float( 0.50f,0.0f,0.0f, 1.0f); }
 void x_CadetBlue()/* X11: 5F9EA0 */{ Float( 0.37f,0.62f,0.63f, 1.0f); }         void x_MediumAquamarine()/* X11: 66CDAA */{ Float( 0.40f,0.80f,0.67f, 1.0f); }
 void x_Chartreuse()/* X11: 7FFF00 */{ Float( 0.50f,1.0f,0.0f, 1.0f); }          void x_MediumBlue()/* X11: 0000CD */{ Float( 0.0f,0.0f,0.80f, 1.0f); }
 void x_Chocolate()/* X11: D2691E */{ Float( 0.82f,0.41f,0.12f, 1.0f); }         void x_MediumOrchid()/* X11: BA55D3 */{ Float( 0.73f,0.33f,0.83f, 1.0f); }
 void x_Coral()/* X11: FF7F50 */{ Float( 1.0f,0.50f,0.31f, 1.0f); }              void x_MediumPurple()/* X11: 9370DB */{ Float( 0.58f,0.44f,0.86f, 1.0f); }
 void x_Cornflower()/* X11: 6495ED */{ Float( 0.39f,0.58f,0.93f, 1.0f); }        void x_MediumSeaGreen()/* X11: 3CB371 */{ Float( 0.24f,0.70f,0.44f, 1.0f); }
 void x_Cornsilk()/* X11: FFF8DC */{ Float( 1.0f,0.97f,0.86f, 1.0f); }           void x_MediumSlateBlue()/* X11: 7B68EE */{ Float( 0.48f,0.41f,0.93f, 1.0f); }
 void x_Crimson()/* X11: DC143C */{ Float( 0.86f,0.8f,0.24f, 1.0f); }            void x_MediumSpringGreen()/* X11: 00FA9A */{ Float( 0.0f,0.98f,0.60f, 1.0f); }
 void x_Cyan()/* X11: 00FFFF */{ Float( 0.0f,1.0f,1.0f, 1.0f); }                void x_MediumTurquoise()/* X11: 48D1CC */{ Float( 0.28f,0.82f,0.80f, 1.0f); }
 void x_DarkBlue()/* X11: 00008B */{ Float( 0.0f,0.0f,0.55f, 1.0f); }            void x_MediumVioletRed()/* X11: C71585 */{ Float( 0.78f,0.8f,0.52f, 1.0f); }
 void x_DarkCyan()/* X11: 008B8B */{ Float( 0.0f,0.55f,0.55f, 1.0f); }           void x_MidnightBlue()/* X11: 191970 */{ Float( 0.10f,0.10f,0.44f, 1.0f); }
 void x_DarkGoldenrod()/* X11: B8860B */{ Float( 0.72f,0.53f,0.4f, 1.0f); }      void x_MintCream()/* X11: F5FFFA */{ Float( 0.96f,1.0f,0.98f, 1.0f); }
 void x_DarkGray()/* X11: A9A9A9 */{ Float( 0.66f,0.66f,0.66f, 1.0f); }          void x_MistyRose()/* X11: FFE4E1 */{ Float( 1.0f,0.89f,0.88f, 1.0f); }
 void x_DarkGreen()/* X11: 006400 */{ Float( 0.0f,0.39f,0.0f, 1.0f); }           void x_Moccasin()/* X11: FFE4B5 */{ Float( 1.0f,0.89f,0.71f, 1.0f); }
 void x_DarkKhaki()/* X11: BDB76B */{ Float( 0.74f,0.72f,0.42f, 1.0f); }         void x_NavajoWhite()/* X11: FFDEAD */{ Float( 1.0f,0.87f,0.68f, 1.0f); }
 void x_DarkMagenta()/* X11: 8B008B */{ Float( 0.55f,0.0f,0.55f, 1.0f); }        void x_NavyBlue()/* X11: 000080 */{ Float( 0.0f,0.0f,0.50f, 1.0f); }
 void x_DarkOliveGreen()/* X11: 556B2F */{ Float( 0.33f,0.42f,0.18f, 1.0f); }    void x_Navy()/* X11: 000080 */{ Float( 0.0f,0.0f,0.50f, 1.0f); }
 void x_DarkOrange()/* X11: FF8C00 */{ Float( 1.0f,0.55f,0.0f, 1.0f); }          void x_OldLace()/* X11: FDF5E6 */{ Float( 0.99f,0.96f,0.90f, 1.0f); }
 void x_DarkOrchid()/* X11: 9932CC */ { Float(0.60f,0.20f,0.80f, 1.0f); }        void x_Olive()/* X11: 808000 */{ Float( 0.50f,0.50f,0.0f, 1.0f); }
 void x_DarkRed()/* X11: 8B0000 */{ Float( 0.55f,0.0f,0.0f, 1.0f); }             void x_OliveDrab()/* X11: 6B8E23 */{ Float( 0.42f,0.56f,0.14f, 1.0f); }
 void x_DarkSalmon()/* X11: E9967A */{ Float( 0.91f,0.59f,0.48f, 1.0f); }        void x_Orange()/* X11: FFA500 */{ Float( 1.0f,0.65f,0.0f, 1.0f); }
 void x_DarkSeaGreen()/* X11: 8FBC8F */{ Float( 0.56f,0.74f,0.56f, 1.0f); }      void x_OrangeRed()/* X11: FF4500 */{ Float( 1.0f,0.27f,0.0f, 1.0f); }
 void x_DarkSlateBlue()/* X11: 483D8B */{ Float( 0.28f,0.24f,0.55f, 1.0f); }     void x_Orchid()/* X11: DA70D6 */{ Float( 0.85f,0.44f,0.84f, 1.0f); }
 void x_DarkSlateGray()/* X11: 2F4F4F */{ Float( 0.18f,0.31f,0.31f, 1.0f); }     void x_PaleGoldenrod()/* X11: EEE8AA */{ Float( 0.93f,0.91f,0.67f, 1.0f); }
 void x_DarkTurquoise()/* X11: 00CED1 */{ Float( 0.0f,0.81f,0.82f, 1.0f); }      void x_PaleGreen()/* X11: 98FB98 */{ Float( 0.60f,0.98f,0.60f, 1.0f); }
 void x_DarkViolet()/* X11: 9400D3 */{ Float( 0.58f,0.0f,0.83f, 1.0f); }         void x_PaleTurquoise()/* X11: AFEEEE */{ Float( 0.69f,0.93f,0.93f, 1.0f); }
 void x_DeepPink()/* X11: FF1493 */{ Float( 1.0f,0.8f,0.58f, 1.0f); }            void x_PaleVioletRed()/* X11: DB7093 */{ Float( 0.86f,0.44f,0.58f, 1.0f); }
 void x_DeepSkyBlue()/* X11: 00BFFF */{ Float( 0.0f,0.75f,1.0f, 1.0f); }        void x_PapayaWhip()/* X11: FFEFD5 */{ Float( 1.0f,0.94f,0.84f, 1.0f); }
 void x_DimGray()/* X11: 696969 */{ Float( 0.41f,0.41f,0.41f, 1.0f); }           void x_PeachPuff()/* X11: FFDAB9 */{ Float( 1.0f,0.85f,0.73f, 1.0f); }
 void x_DodgerBlue()/* X11: 1E90FF */{ Float( 0.12f,0.56f,1.0f, 1.0f); }        void x_Peru()/* X11: CD853F */{ Float( 0.80f,0.52f,0.25f, 1.0f); }
 void x_Firebrick()/* X11: B22222 */{ Float( 0.70f,0.13f,0.13f, 1.0f); }         void x_Pink()/* X11: FFC0CB */{ Float( 1.0f,0.75f,0.80f, 1.0f); }
 void x_FloralWhite()/* X11: FFFAF0 */{ Float( 1.0f,0.98f,0.94f, 1.0f); }        void x_Plum()/* X11: DDA0DD */{ Float( 0.87f,0.63f,0.87f, 1.0f); }
 void x_ForestGreen()/* X11: 228B22 */{ Float( 0.13f,0.55f,0.13f, 1.0f); }       void x_PowderBlue()/* X11: B0E0E6 */{ Float( 0.69f,0.88f,0.90f, 1.0f); }
 void x_fuchsia()/* X11: FF00FF */{ Float( 1.0f,0.0f,1.0f, 1.0f); }             void x_Purple()/* X11: A020F0 */{ Float( 0.63f,0.13f,0.94f, 1.0f); }
 void x_Gainsboro()/* X11: DCDCDC */{ Float( 0.86f,0.86f,0.86f, 1.0f); }         void x_WebPurple()/* X11: 7F007F */{ Float( 0.50f,0.0f,0.50f, 1.0f); }
 void x_GhostWhite()/* X11: F8F8FF */{ Float( 0.97f,0.97f,1.0f, 1.0f); }        void x_RebeccaPurple()/* X11: 663399 */{ Float( 0.40f,0.20f,0.60f, 1.0f); }
 void x_Gold()/* X11: FFD700 */{ Float( 1.0f,0.84f,0.0f, 1.0f); }                void x_Red()/* X11: FF0000 */{ Float( 1.0f,0.0f,0.0f, 1.0f); }
 void x_Goldenrod()/* X11: DAA520 */{ Float( 0.85f,0.65f,0.13f, 1.0f); }         void x_RosyBrown()/* X11: BC8F8F */{ Float( 0.74f,0.56f,0.56f, 1.0f); }
 void x_Gray()/* X11: BEBEBE */{ Float( 0.75f,0.75f,0.75f, 1.0f); }              void x_RoyalBlue()/* X11: 4169E1 */{ Float( 0.25f,0.41f,0.88f, 1.0f); }
 void x_Grey()/* X11: BEBEBE */{ Float( 0.75f,0.75f,0.75f, 1.0f); }              void x_SaddleBrown()/* X11: 8B4513 */{ Float( 0.55f,0.27f,0.7f, 1.0f); }
 void x_WebGray()/* X11: 808080 */{ Float( 0.50f,0.50f,0.50f, 1.0f); }           void x_Salmon()/* X11: FA8072 */{ Float( 0.98f,0.50f,0.45f, 1.0f); }
 void x_Green()/* X11: 00FF00 */{ Float( 0.0f,1.0f,0.0f, 1.0f); }                void x_SandyBrown()/* X11: F4A460 */{ Float( 0.96f,0.64f,0.38f, 1.0f); }
 void x_lime()/* X11: 00FF00 */{ Float( 0.0f,1.0f,0.0f, 1.0f); }                 void x_SeaGreen()/* X11: 2E8B57 */{ Float( 0.18f,0.55f,0.34f, 1.0f); }
 void x_WebGreen()/* X11: 008000 */{ Float( 0.0f,0.50f,0.0f, 1.0f); }            void x_Seashell()/* X11: FFF5EE */{ Float( 1.0f,0.96f,0.93f, 1.0f); }
 void x_GreenYellow()/* X11: ADFF2F */{ Float( 0.68f,1.0f,0.18f, 1.0f); }        void x_Sienna()/* X11: A0522D */{ Float( 0.63f,0.32f,0.18f, 1.0f); }
 void x_Honeydew()/* X11: F0FFF0 */{ Float( 0.94f,1.0f,0.94f, 1.0f); }           void x_Silver()/* X11: C0C0C0 */{ Float( 0.75f,0.75f,0.75f, 1.0f); }
 void x_HotPink()/* X11: FF69B4 */{ Float( 1.0f,0.41f,0.71f, 1.0f); }            void x_SkyBlue()/* X11: 87CEEB */{ Float( 0.53f,0.81f,0.92f, 1.0f); }
 void x_IndianRed()/* X11: CD5C5C */{ Float( 0.80f,0.36f,0.36f, 1.0f); }         void x_SlateBlue()/* X11: 6A5ACD */{ Float( 0.42f,0.35f,0.80f, 1.0f); }
 void x_Indigo()/* X11: 4B0082 */{ Float( 0.29f,0.0f,0.51f, 1.0f); }             void x_SlateGray()/* X11: 708090 */{ Float( 0.44f,0.50f,0.56f, 1.0f); }
 void x_Ivory()/* X11: FFFFF0 */{ Float( 1.0f,1.0f,0.94f, 1.0f); }               void x_SlateGrey()/* X11: 708090 */{ Float( 0.44f,0.50f,0.56f, 1.0f); }
 void x_Khaki()/* X11: F0E68C */{ Float( 0.94f,0.90f,0.55f, 1.0f); }             void x_Snow()/* X11: FFFAFA */{ Float( 1.0f,0.98f,0.98f, 1.0f); }
 void x_Lavender()/* X11: E6E6FA */{ Float( 0.90f,0.90f,0.98f, 1.0f); }          void x_SpringGreen()/* X11: 00FF7F */{ Float( 0.0f,1.0f,0.50f, 1.0f); }
 void x_LavenderBlush()/* X11: FFF0F5 */{ Float( 1.0f,0.94f,0.96f, 1.0f); }      void x_SteelBlue()/* X11: 4682B4 */{ Float( 0.27f,0.51f,0.71f, 1.0f); }
 void x_LawnGreen()/* X11: 7CFC00 */{ Float( 0.49f,0.99f,0.0f, 1.0f); }          void x_Tan()/* X11: D2B48C */{ Float( 0.82f,0.71f,0.55f, 1.0f); }
 void x_LemonChiffon()/* X11: FFFACD */{ Float( 1.0f,0.98f,0.80f, 1.0f); }       void x_Teal()/* X11: 008080 */{ Float( 0.0f,0.50f,0.50f, 1.0f); }
 void x_LightBlue()/* X11: ADD8E6 */{ Float( 0.68f,0.85f,0.90f, 1.0f); }         void x_Thistle()/* X11: D8BFD8 */{ Float( 0.85f,0.75f,0.85f, 1.0f); }
 void x_LightCoral()/* X11: F08080 */{ Float( 0.94f,0.50f,0.50f, 1.0f); }        void x_Tomato()/* X11: FF6347 */{ Float( 1.0f,0.39f,0.28f, 1.0f); }
 void x_LightCyan()/* X11: E0FFFF */{ Float( 0.88f,1.0f,1.0f, 1.0f); }          void x_Turquoise()/* X11: 40E0D0 */{ Float( 0.25f,0.88f,0.82f, 1.0f); }
 void x_LightGoldenrod()/* X11: FAFAD2 */{ Float( 0.98f,0.98f,0.82f, 1.0f); }    void x_Violet()/* X11: EE82EE */{ Float( 0.93f,0.51f,0.93f, 1.0f); }
 void x_LightGray()/* X11: D3D3D3 */{ Float( 0.83f,0.83f,0.83f, 1.0f); }         void x_Wheat()/* X11: F5DEB3 */{ Float( 0.96f,0.87f,0.70f, 1.0f); }
 void x_LightGrey()/* X11: D3D3D3 */{ Float( 0.83f,0.83f,0.83f, 1.0f); }         void x_White()/* X11: FFFFFF */{ Float( 1.0f,1.0f,1.0f, 1.0f); }
 void x_LightGreen()/* X11: 90EE90 */{ Float( 0.56f,0.93f,0.56f, 1.0f); }        void x_WhiteSmoke()/* X11: F5F5F5 */{ Float( 0.96f,0.96f,0.96f, 1.0f); }
 void x_LightPink()/* X11: FFB6C1 */{ Float( 1.0f,0.71f,0.76f, 1.0f); }          void x_Yellow()/* X11: FFFF00 */{ Float( 1.0f,1.0f,0.0f, 1.0f); }
 void x_LightSalmon()/* X11: FFA07A */{ Float( 1.0f,0.63f,0.48f, 1.0f); }        void x_YellowGreen()/* X11: 9ACD32 */{ Float( 0.60f,0.80f,0.20f, 1.0f); }
 // CSS Color Presets
 void z_white() /*CSS 2.1 #FFFFFF */  { Int(255,255,255,255); }                void z_green() /*CSS 2.1 #008000 */  { Int(0,128,0    ,255); }
 void z_gray() /*CSS 2.1 #808080 */  { Int(128,128,128,255); }                 void z_darkgreen() /*CSS 2.1 #006400 */  { Int(0,100,0    ,255); }
 void z_silver() /*CSS 2.1 #C0C0C0 */  { Int(192,192,192,255); }               void z_yellowgreen() /*CSS 2.1 #9ACD32 */  { Int(154,205,50 ,255); }
 void z_black() /*CSS 2.1 #000000 */  { Int(0,0,0      ,255); }                void z_olivedrab() /*CSS 2.1 #6B8E23 */  { Int(107,142,35 ,255); }
 void z_maroon() /*CSS 2.1 #800000 */  { Int(128,0,0    ,255); }               void z_olive() /*CSS 2.1 #808000 */  { Int(128,128,0  ,255); }
 void z_red() /*CSS 2.1 #FF0000 */  { Int(255,0,0    ,255); }                  void z_darkolivegreen() /*CSS 2.1 #556B2F */  { Int(85,107,47  ,255); }
 void z_purple() /*CSS 2.1 #800080 */  { Int(128,0,128  ,255); }               void z_mediumaquamarine() /*CSS 2.1 #66CDAA */  { Int(102,205,170,255); }
 void z_fuchsia() /*CSS 2.1 #FF00FF */  { Int(255,0,255  ,255); }              void z_darkseagreen() /*CSS 2.1 #8FBC8F */  { Int(143,188,143,255); }
 void z_Green() /*CSS 2.1 #008000 */  { Int(0,128,0    ,255); }                void z_lightseagreen() /*CSS 2.1 #20B2AA */  { Int(32,178,170 ,255); }
 void z_lime() /*CSS 2.1 #00FF00 */  { Int(0,255,0    ,255); }                 void z_darkcyan() /*CSS 2.1 #008B8B */  { Int(0,139,139  ,255); }
 void z_Olive() /*CSS 2.1 #808000 */  { Int(128,128,0  ,255); }                void z_teal() /*CSS 2.1 #008080 */  { Int(0,128,128  ,255); }
 void z_yellow() /*CSS 2.1 #FFFF00 */  { Int(255,255,0  ,255); }               void z_aqua() /*CSS 2.1 #00FFFF */  { Int(0,255,255  ,255); }
 void z_navy() /*CSS 2.1 #000080 */  { Int(0,0,128    ,255); }                 void z_cyan() /*CSS 2.1 #00FFFF */  { Int(0,255,255  ,255); }
 void z_blue() /*CSS 2.1 #0000FF */  { Int(0,0,255    ,255); }                 void z_lightcyan() /*CSS 2.1 #E0FFFF */  { Int(224,255,255,255); }
 void z_Teal() /*CSS 2.1 #008080 */  { Int(0,128,128  ,255); }                 void z_paleturquoise() /*CSS 2.1 #AFEEEE */  { Int(175,238,238,255); }
 void z_Aqua() /*CSS 2.1 #00FFFF */  { Int(0,255,255  ,255); }                 void z_aquamarine() /*CSS 2.1 #7FFFD4 */  { Int(127,255,212,255); }
 void z_Orange() /*CSS 2.1 #FFA500 */  { Int(255,165,0  ,255); }               void z_turquoise() /*CSS 2.1 #40E0D0 */  { Int(64,224,208 ,255); }
 void z_indianred() /*CSS 2.1 #CD5C5C */  { Int(205,92,92  ,255); }            void z_mediumturquoise() /*CSS 2.1 #48D1CC */  { Int(72,209,204 ,255); }
 void z_lightcoral() /*CSS 2.1 #F08080 */  { Int(240,128,128,255); }           void z_darkturquoise() /*CSS 2.1 #00CED1 */  { Int(0,206,209  ,255); }
 void z_salmon() /*CSS 2.1 #FA8072 */  { Int(250,128,114,255); }               void z_cadetblue() /*CSS 2.1 #5F9EA0 */  { Int(95,158,160 ,255); }
 void z_darksalmon() /*CSS 2.1 #E9967A */  { Int(233,150,122,255); }           void z_steelblue() /*CSS 2.1 #4682B4 */  { Int(70,130,180 ,255); }
 void z_lightsalmon() /*CSS 2.1 #FFA07A */  { Int(255,160,122,255); }          void z_lightsteelblue() /*CSS 2.1 #B0C4DE */  { Int(176,196,222,255); }
 void z_crimson() /*CSS 2.1 #DC143C */  { Int(220,20,60  ,255); }              void z_powderblue() /*CSS 2.1 #B0E0E6 */  { Int(176,224,230,255); }
 void z_Red() /*CSS 2.1 #FF0000 */  { Int(255,0,0    ,255); }                  void z_lightblue() /*CSS 2.1 #ADD8E6 */  { Int(173,216,230,255); }
 void z_firebrick() /*CSS 2.1 #B22222 */  { Int(178,34,34  ,255); }            void z_skyblue() /*CSS 2.1 #87CEEB */  { Int(135,206,235,255); }
 void z_darkred() /*CSS 2.1 #8B0000 */  { Int(139,0,0    ,255); }              void z_lightskyblue() /*CSS 2.1 #87CEFA */  { Int(135,206,250,255); }
 void z_pink() /*CSS 2.1 #FFC0CB */  { Int(255,192,203,255); }                 void z_deepskyblue() /*CSS 2.1 #00BFFF */  { Int(0,191,255  ,255); }
 void z_lightpink() /*CSS 2.1 #FFB6C1 */  { Int(255,182,193,255); }            void z_dodgerblue() /*CSS 2.1 #1E90FF */  { Int(30,144,255 ,255); }
 void z_hotpink() /*CSS 2.1 #FF69B4 */  { Int(255,105,180,255); }              void z_cornflowerblue() /*CSS 2.1 #6495ED */  { Int(100,149,237,255); }
 void z_deeppink() /*CSS 2.1 #FF1493 */  { Int(255,20,147 ,255); }             void z_mediumslateblue() /*CSS 2.1 #7B68EE */  { Int(123,104,238,255); }
 void z_mediumvioletred() /*CSS 2.1 #C71585 */  { Int(199,21,133 ,255); }      void z_royalblue() /*CSS 2.1 #4169E1 */  { Int(65,105,225 ,255); }
 void z_palevioletred() /*CSS 2.1 #DB7093 */  { Int(219,112,147,255); }        void z_Blue() /*CSS 2.1 #0000FF */  { Int(0,0,255    ,255); }
 void z_Lightsalmon() /*CSS 2.1 #FFA07A */  { Int(255,160,122,255); }          void z_mediumblue() /*CSS 2.1 #0000CD */  { Int(0,0,205    ,255); }
 void z_coral() /*CSS 2.1 #FF7F50 */  { Int(255,127,80 ,255); }                void z_darkblue() /*CSS 2.1 #00008B */  { Int(0,0,139    ,255); }
 void z_tomato() /*CSS 2.1 #FF6347 */  { Int(255,99,71  ,255); }               void z_Navy() /*CSS 2.1 #000080 */  { Int(0,0,128    ,255); }
 void z_orangered() /*CSS 2.1 #FF4500 */  { Int(255,69,0   ,255); }            void z_midnightblue() /*CSS 2.1 #191970 */  { Int(25,25,112  ,255); }
 void z_darkorange() /*CSS 2.1 #FF8C00 */  { Int(255,140,0  ,255); }           void z_cornsilk() /*CSS 2.1 #FFF8DC */  { Int(255,248,220,255); }
 void z_orange() /*CSS 2.1 #FFA500 */  { Int(255,165,0  ,255); }               void z_blanchedalmond() /*CSS 2.1 #FFEBCD */  { Int(255,235,205,255); }
 void z_gold() /*CSS 2.1 #FFD700 */  { Int(255,215,0  ,255); }                 void z_bisque() /*CSS 2.1 #FFE4C4 */  { Int(255,228,196,255); }
 void z_Yellow() /*CSS 2.1 #FFFF00 */  { Int(255,255,0  ,255); }               void z_navajowhite() /*CSS 2.1 #FFDEAD */  { Int(255,222,173,255); }
 void z_lightyellow() /*CSS 2.1 #FFFFE0 */  { Int(255,255,224,255); }          void z_wheat() /*CSS 2.1 #F5DEB3 */  { Int(245,222,179,255); }
 void z_lemonchiffon() /*CSS 2.1 #FFFACD */  { Int(255,250,205,255); }         void z_burlywood() /*CSS 2.1 #DEB887 */  { Int(222,184,135,255); }
 void z_lightgoldenrodyellow() /*CSS 2.1 #FAFAD2 */  { Int(250,250,210,255); } void z_tan() /*CSS 2.1 #D2B48C */  { Int(210,180,140,255); }
 void z_papayawhip() /*CSS 2.1 #FFEFD5 */  { Int(255,239,213,255); }           void z_rosybrown() /*CSS 2.1 #BC8F8F */  { Int(188,143,143,255); }
 void z_moccasin() /*CSS 2.1 #FFE4B5 */  { Int(255,228,181,255); }             void z_sandybrown() /*CSS 2.1 #F4A460 */  { Int(244,164,96 ,255); }
 void z_peachpuff() /*CSS 2.1 #FFDAB9 */  { Int(255,218,185,255); }            void z_goldenrod() /*CSS 2.1 #DAA520 */  { Int(218,165,32 ,255); }
 void z_palegoldenrod() /*CSS 2.1 #EEE8AA */  { Int(238,232,170,255); }        void z_darkgoldenrod() /*CSS 2.1 #B8860B */  { Int(184,134,11 ,255); }
 void z_khaki() /*CSS 2.1 #F0E68C */  { Int(240,230,140,255); }                void z_peru() /*CSS 2.1 #CD853F */  { Int(205,133,63 ,255); }
 void z_darkkhaki() /*CSS 2.1 #BDB76B */  { Int(189,183,107,255); }            void z_chocolate() /*CSS 2.1 #D2691E */  { Int(210,105,30 ,255); }
 void z_lavender() /*CSS 2.1 #E6E6FA */  { Int(230,230,250,255); }             void z_saddlebrown() /*CSS 2.1 #8B4513 */  { Int(139,69,19  ,255); }
 void z_thistle() /*CSS 2.1 #D8BFD8 */  { Int(216,191,216,255); }              void z_sienna() /*CSS 2.1 #A0522D */  { Int(160,82,45  ,255); }
 void z_plum() /*CSS 2.1 #DDA0DD */  { Int(221,160,221,255); }                 void z_brown() /*CSS 2.1 #A52A2A */  { Int(165,42,42  ,255); }
 void z_violet() /*CSS 2.1 #EE82EE */  { Int(238,130,238,255); }               void z_Maroon() /*CSS 2.1 #800000 */  { Int(128,0,0    ,255); }
 void z_orchid() /*CSS 2.1 #DA70D6 */  { Int(218,112,214,255); }               void z_White() /*CSS 2.1 #FFFFFF */  { Int(255,255,255,255); }
 void z_Fuchsia() /*CSS 2.1 #FF00FF */  { Int(255,0,255  ,255); }              void z_snow() /*CSS 2.1 #FFFAFA */  { Int(255,250,250,255); }
 void z_magenta() /*CSS 2.1 #FF00FF */  { Int(255,0,255  ,255); }              void z_honeydew() /*CSS 2.1 #F0FFF0 */  { Int(240,255,240,255); }
 void z_mediumorchid() /*CSS 2.1 #BA55D3 */  { Int(186,85,211 ,255); }         void z_mintcream() /*CSS 2.1 #F5FFFA */  { Int(245,255,250,255); }
 void z_mediumpurple() /*CSS 2.1 #9370DB */  { Int(147,112,219,255); }         void z_azure() /*CSS 2.1 #F0FFFF */  { Int(240,255,255,255); }
 void z_blueviolet() /*CSS 2.1 #8A2BE2 */  { Int(138,43,226 ,255); }           void z_aliceblue() /*CSS 2.1 #F0F8FF */  { Int(240,248,255,255); }
 void z_darkviolet() /*CSS 2.1 #9400D3 */  { Int(148,0,211  ,255); }           void z_ghostwhite() /*CSS 2.1 #F8F8FF */  { Int(248,248,255,255); }
 void z_darkorchid() /*CSS 2.1 #9932CC */  { Int(153,50,204 ,255); }           void z_whitesmoke() /*CSS 2.1 #F5F5F5 */  { Int(245,245,245,255); }
 void z_darkmagenta() /*CSS 2.1 #8B008B */  { Int(139,0,139  ,255); }          void z_seashell() /*CSS 2.1 #FFF5EE */  { Int(255,245,238,255); }
 void z_Purple() /*CSS 2.1 #800080 */  { Int(128,0,128  ,255); }               void z_beige() /*CSS 2.1 #F5F5DC */  { Int(245,245,220,255); }
 void z_indigo() /*CSS 2.1 #4B0082 */  { Int(75,0,130   ,255); }               void z_oldlace() /*CSS 2.1 #FDF5E6 */  { Int(253,245,230,255); }
 void z_slateblue() /*CSS 2.1 #6A5ACD */  { Int(106,90,205 ,255); }            void z_floralwhite() /*CSS 2.1 #FFFAF0 */  { Int(255,250,240,255); }
 void z_darkslateblue() /*CSS 2.1 #483D8B */  { Int(72,61,139  ,255); }        void z_ivory() /*CSS 2.1 #FFFFF0 */  { Int(255,255,240,255); }
 void z_Mediumslateblue() /*CSS 2.1 #7B68EE */  { Int(123,104,238,255); }      void z_antiquewhite() /*CSS 2.1 #FAEBD7 */  { Int(250,235,215,255); }
 void z_greenyellow() /*CSS 2.1 #ADFF2F */  { Int(173,255,47 ,255); }          void z_linen() /*CSS 2.1 #FAF0E6 */  { Int(250,240,230,255); }
 void z_chartreuse() /*CSS 2.1 #7FFF00 */  { Int(127,255,0  ,255); }           void z_lavenderblush() /*CSS 2.1 #FFF0F5 */  { Int(255,240,245,255); }
 void z_lawngreen() /*CSS 2.1 #7CFC00 */  { Int(124,252,0  ,255); }            void z_mistyrose() /*CSS 2.1 #FFE4E1 */  { Int(255,228,225,255); }
 void z_Lime() /*CSS 2.1 #00FF00 */  { Int(0,255,0    ,255); }                 void z_gainsboro() /*CSS 2.1 #DCDCDC */  { Int(220,220,220,255); }
 void z_limegreen() /*CSS 2.1 #32CD32 */  { Int(50,205,50  ,255); }            void z_lightgrey() /*CSS 2.1 #D3D3D3 */  { Int(211,211,211,255); }
 void z_palegreen() /*CSS 2.1 #98FB98 */  { Int(152,251,152,255); }            void z_Silver() /*CSS 2.1 #C0C0C0 */  { Int(192,192,192,255); }
 void z_lightgreen() /*CSS 2.1 #90EE90 */  { Int(144,238,144,255); }           void z_darkgray() /*CSS 2.1 #A9A9A9 */  { Int(169,169,169,255); }
 void z_mediumspringgreen() /*CSS 2.1 #00FA9A */  { Int(0,250,154  ,255); }    void z_Gray() /*CSS 2.1 #808080 */  { Int(128,128,128,255); }
 void z_springgreen() /*CSS 2.1 #00FF7F */  { Int(0,255,127  ,255); }          void z_dimgray() /*CSS 2.1 #696969 */  { Int(105,105,105,255); }
 void z_mediumseagreen() /*CSS 2.1 #3CB371 */  { Int(60,179,113 ,255); }       void z_lightslategray() /*CSS 2.1 #778899 */  { Int(119,136,153,255); }
 void z_seagreen() /*CSS 2.1 #2E8B57 */  { Int(46,139,87  ,255); }             void z_slategray() /*CSS 2.1 #708090 */  { Int(112,128,144,255); }
 void z_forestgreen() /*CSS 2.1 #228B22 */  { Int(34,139,34  ,255); }          void z_darkslategray() /*CSS 2.1 #2F4F4F */  { Int(47,79,79   ,255); }
};

bool operator== ( const Crayon& a, const Crayon& b );
bool operator!= ( const Crayon& a, const Crayon& b );
Crayon operator+ ( const Crayon& a, const Crayon& b );
Crayon operator* ( const Crayon& a, const Crayon& b );

class CrayonBox 
{
public:
 Indexed<Crayon> pick; // #=last color+1
 Indexed<Crayon> bright;
 Indexed<Crayon> vivid;
 Indexed<Crayon> betweenFingers;

 inline Crayon Any() {
  return pick[upto(100000+TOTAL_COLORS)%(TOTAL_COLORS)];
 }
 inline Crayon Any(int seed) {
  return pick[upto(100000+TOTAL_COLORS,seed)%(TOTAL_COLORS)];
 }
 inline Crayon AnySolid() {
  Crayon c;
  c.Pick(clear);
  while ( c.floats[3]<1.0f ) c.fromCrayon(pick[upto(TOTAL_COLORS)]);
  return c;
 }
 inline Crayon SeededSolid( int seed ) {
  Crayon c;
  c.Pick((Colors)(seed%TOTAL_COLORS));
  while ( c.floats[3]<1.0f ) c.fromCrayon(pick[(++seed)]);
  return c;
 }
 inline Crayon SolidSeeded( int seed ) {
  Crayon c;
  c.Float(xyzs[seed%32],xyzs[seed%33],xyzs[seed%34],1.0f);
  return c;
 }
 inline Crayon Vivids(int seed) {
  return vivid[(unsigned int)(seed%TOTAL_BRIGHTS)];
 }
 inline Crayon Vivid(unsigned int index) {
  return vivid(index);
 }
 inline Crayon Brights(int seed) {
  return bright[(unsigned int)(seed%TOTAL_BRIGHTS)];
 }
 inline Crayon Bright(unsigned int index) {
  return bright(index);
 }
 inline Crayon Offwhite(int seed) {
  Crayon drawing;
  drawing.Float(
   0.75f+0.25f*xyzs[seed%20000],
   0.75f+0.25f*xyzs[seed%20000+20000],
   0.75f+0.25f*xyzs[seed%20000+40000],
   1.0f);
  return drawing;
 }
// Note: in Crayon Pick() I did not use () for speed, but creates overreads breaking Release mode in statics, so in a static constructor do not use
  inline Crayon Pick( Colors c ) { return pick[(Colors) c]; }
 void gl( Colors c );

 // 1-8 crayons stuck between your fingers for passing to functions in draw (decreases development time, may not be lowest cycles?)
 inline Crayon jami( int id, int r, int g, int b, int a ) {
  betweenFingers[id].Int( r,g,b,a );  
  return betweenFingers[id];
 }
 inline Crayon jam( int id, float r, float g, float b, float a ) {
  betweenFingers[id].Float( r,g,b,a );
  return betweenFingers[id];
 }
 CrayonBox();

 Crayon fansi( int i );
 int fansi( Crayon c );

 Crayon c16x16( int i );
 int c16x16( Crayon c );
};

extern CrayonBox crayons;

ONE(CrayonHandle,{}) // NamedHandle didn't work?!
 Crayon color;
 string toString() { return color.toCSS(); }
 void fromString( const char *s ) {
  color.CSS(s);
 }
 BINARY_IN({
  B_IN_WORD({fromString(w.c_str());})
 })
 BINARY_OUT({
  B_OUT_WORD(toString().c_str())
 })
MANY(CrayonHandle,CrayonHandleHandle,CrayonHandleHandles,"tint",Crayons,{
  nearness=1.0f/255.0f;
})
 float nearness;
 Crayon melted;
 Crayon Color() { CrayonHandle *ch=(CrayonHandle *) this->Any(); if ( ch ) return ch->color; return crayons.Any(); }
 void Seeded(int seed,Crayon *out,Crayon defaultColor=crayons.Pick(alabaster)) {
  if ( count.value == 0 ) { out->fromCrayon(defaultColor); }
  else {
   CrayonHandle *t=(CrayonHandle *) Element(seed%count.value);
   if ( !t ) out->fromCrayon(defaultColor);
   else out->fromCrayon(t->color);
  }
 }
 void ReadAppend( BinaryFile *open );
 void Melt( float f ) {
  int g=(int) (f*1000000);
  int i=(int) (g/100000)%count; // largest 1,000,000,000-0
  CrayonHandle *c=(CrayonHandle *) Element(i), *d;
  if ( i == 0 ) d=(CrayonHandle *) last;
  else d=(CrayonHandle *) Element( i-1 );
  melted.ColorInterp(c->color,d->color,(f-1.0f/(float)count)*(float)count);
 }
 void Hold( Crayon c ) {
  if ( !nearby(c,nearness) ) {
   CrayonHandle *crayon=new CrayonHandle;
   crayon->color.fromCrayon(c);
   Append(crayon);
  }
 }
 void Add( Crayon c ) { Hold(c); }
 void Add( Colors c ) { Hold(c); }
 void Hold( Colors c );
 void Hold( Crayons *s ) { EACH(s->first,CrayonHandle,c) if ( !nearby(c->color,nearness) ) Hold(c->color); }
 Crayon random() {
  if ( count == 1 ) return ((CrayonHandle *) first)->color;
  CrayonHandle *c=(CrayonHandle *) Element(upto(count));
  if ( !c ) return crayons.Any();
  return c->color;
 }
 bool nearby( Crayon in, float nearness ) {
  FOREACH(CrayonHandle,h) if ( NEARBY(in.rf,h->color.rf,nearness)
                            && NEARBY(in.gf,h->color.gf,nearness)
                            && NEARBY(in.bf,h->color.bf,nearness)
                            && NEARBY(in.af,h->color.af,nearness) ) {
   return true;
  }
  return false;
 }
 KEYWORDSGroup("tint",CrayonHandle);
 WORDKEYSGroup("tint",CrayonHandle);
 BINARY_INOUT(CrayonHandle)
DONE(CrayonHandle);