SET Language=1033
FOR /f "tokens=3" %%a in ('REG QUERY "HKCU\Control Panel\International" /v sCountry ^| find /i "sCountry") do echo %%a | findstr /b /i "Turkey" 
IF NOT ERRORLEVEL 1 Set Language=1055

for /f "tokens=2 delims==" %%A in ('wmic path win32_OperatingSystem get OSLanguage /Value') do set Language=%%A

goto %Language%

rem English
:1033
:2067
:11273
:7177
:5129
:8201
:6153
:4105
:10249
:3081


cd %~dp0
REF_FONT_SIZE=8
start /b /max viewer.exe Apolune_README.txt

goto end

rem Turkish
:1055

start /b /max notepad.exe Apolune_README_Unicode_Turkish.txt

goto end
:ignore


Language Codes
Lookup tables for language codes as used in the Windows registry (HKEY_CURRENT_USER\Control Panel\International\Locale).
In VBScript the code for the local computer can be obtained with the GetLocale( ) function.

Sorted by (English) Name
Language	Decimal Value
Afrikaans	1078
Albanian	1052
Arabic (Algeria)	5121
Arabic (Bahrain)	15361
Arabic (Egypt)	3073
Arabic (Iraq)	2049
Arabic (Jordan)	11265
Arabic (Kuwait)	13313
Arabic (Lebanon)	12289
Arabic (Libya)	4097
Arabic (Morocco)	6145
Arabic (Oman)	8193
Arabic (Qatar)	16385
Arabic (Saudi Arabia)	1025
Arabic (Syria)	10241
Arabic (Tunisia)	7169
Arabic (U.A.E.)	14337
Arabic (Yemen)	9217
Basque	1069
Belarusian	1059
Bulgarian	1026
Catalan	1027
Chinese (Hong Kong SAR)	3076
Chinese (PRC)	2052
Chinese (Singapore)	4100
Chinese (Taiwan)	1028
Croatian	1050
Czech	1029
Danish	1030
Dutch	1043
Dutch (Belgium)	2067
English (Australia)	3081
English (Belize)	10249
English (Canada)	4105
English (Ireland)	6153
English (Jamaica)	8201
English (New Zealand)	5129
English (South Africa)	7177
English (Trinidad)	11273
English (United Kingdom)	2057
English (United States)	1033
Estonian	1061
Faeroese	1080
Farsi	1065
Finnish	1035
French (Standard)	1036
French (Belgium)	2060
French (Canada)	3084
French (Luxembourg)	5132
French (Switzerland)	4108
Gaelic (Scotland)	1084
German (Standard)	1031
German (Austrian)	3079
German (Liechtenstein)	5127
German (Luxembourg)	4103
German (Switzerland)	2055
Greek	1032
Hebrew	1037
Hindi	1081
Hungarian	1038
Icelandic	1039
Indonesian	1057
Italian (Standard)	1040
Italian (Switzerland)	2064
Japanese	1041
Korean	1042
Latvian	1062
Lithuanian	1063
Macedonian (FYROM)	1071
Malay (Malaysia)	1086
Maltese	1082
Norwegian (Bokmål)	1044
Polish	1045
Portuguese (Brazil)	1046
Portuguese (Portugal)	2070
Raeto (Romance)	1047
Romanian	1048
Romanian (Moldova)	2072
Russian	1049
Russian (Moldova)	2073
Serbian (Cyrillic)	3098
Setsuana	1074
Slovak	1051
Slovenian	1060
Sorbian	1070
Spanish (Argentina)	11274
Spanish (Bolivia)	16394
Spanish (Chile)	13322
Spanish (Columbia)	9226
Spanish (Costa Rica)	5130
Spanish (Dominican Republic)	7178
Spanish (Ecuador)	12298
Spanish (El Salvador)	17418
Spanish (Guatemala)	4106
Spanish (Honduras)	18442
Spanish (Mexico)	2058
Spanish (Nicaragua)	19466
Spanish (Panama)	6154
Spanish (Paraguay)	15370
Spanish (Peru)	10250
Spanish (Puerto Rico)	20490
Spanish (Spain)	1034
Spanish (Uruguay)	14346
Spanish (Venezuela)	8202
Sutu	1072
Swedish	1053
Swedish (Finland)	2077
Thai	1054
Turkish	1055
Tsonga	1073
Ukranian	1058
Urdu (Pakistan)	1056
Vietnamese	1066
Xhosa	1076
Yiddish	1085
Zulu	1077 
Sorted by Number
Decimal Value	Language
1025	Arabic (Saudi Arabia)
1026	Bulgarian
1027	Catalan
1028	Chinese (Taiwan)
1029	Czech
1030	Danish
1031	German (Standard)
1032	Greek
1033	English (United States)
1034	Spanish (Spain)
1035	Finnish
1036	French (Standard)
1037	Hebrew
1038	Hungarian
1039	Icelandic
1040	Italian (Standard)
1041	Japanese
1042	Korean
1043	Dutch
1044	Norwegian (Bokmål)
1045	Polish
1046	Portuguese (Brazil)
1047	Raeto (Romance)
1048	Romanian
1049	Russian
1050	Croatian
1051	Slovak
1052	Albanian
1053	Swedish
1054	Thai
1055	Turkish
1056	Urdu (Pakistan)
1057	Indonesian
1058	Ukranian
1059	Belarusian
1060	Slovenian
1061	Estonian
1062	Latvian
1063	Lithuanian
1065	Farsi
1066	Vietnamese
1069	Basque
1070	Sorbian
1071	Macedonian (FYROM)
1072	Sutu
1073	Tsonga
1074	Setsuana
1076	Xhosa
1077	Zulu
1078	Afrikaans
1080	Faeroese
1081	Hindi
1082	Maltese
1084	Gaelic (Scotland)
1085	Yiddish
1086	Malay (Malaysia)
2049	Arabic (Iraq)
2052	Chinese (PRC)
2055	German (Switzerland)
2057	English (United Kingdom)
2058	Spanish (Mexico)
2060	French (Belgium)
2064	Italian (Switzerland)
2067	Dutch (Belgium)
2070	Portuguese (Portugal)
2072	Romanian (Moldova)
2073	Russian (Moldova)
2077	Swedish (Finland)
3073	Arabic (Egypt)
3076	Chinese (Hong Kong SAR)
3079	German (Austrian)
3081	English (Australia)
3084	French (Canada)
3098	Serbian (Cyrillic)
4097	Arabic (Libya)
4100	Chinese (Singapore)
4103	German (Luxembourg)
4105	English (Canada)
4106	Spanish (Guatemala)
4108	French (Switzerland)
5121	Arabic (Algeria)
5127	German (Liechtenstein)
5129	English (New Zealand)
5130	Spanish (Costa Rica)
5132	French (Luxembourg)
6145	Arabic (Morocco)
6153	English (Ireland)
6154	Spanish (Panama)
7169	Arabic (Tunisia)
7177	English (South Africa)
7178	Spanish (Dominican Republic)
8193	Arabic (Oman)
8201	English (Jamaica)
8202	Spanish (Venezuela)
9217	Arabic (Yemen)
9226	Spanish (Columbia)
10241	Arabic (Syria)
10249	English (Belize)
10250	Spanish (Peru)
11265	Arabic (Jordan)
11273	English (Trinidad)
11274	Spanish (Argentina)
12289	Arabic (Lebanon)
12298	Spanish (Ecuador)
13313	Arabic (Kuwait)
13322	Spanish (Chile)
14337	Arabic (U.A.E.)
14346	Spanish (Uruguay)
15361	Arabic (Bahrain)
15370	Spanish (Paraguay)
16385	Arabic (Qatar)
16394	Spanish (Bolivia)
17418	Spanish (El Salvador)
18442	Spanish (Honduras)
19466	Spanish (Nicaragua)
20490	Spanish (Puerto Rico)

:end