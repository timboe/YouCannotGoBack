rm -r YouCannotGoBack.pdx

sed 's|//#define SCOREBOARD|#define SCOREBOARD|g' src/game.h --in-place=.bak
rm src/*bak

cp Source/titles/card_c.png Source/titles/card.png 

make clean
make
rm YouCannotGoBack.pdx/titles/*_i.pdi
rm YouCannotGoBack.pdx/titles/*_c.pdi
zip -r -q YouCannotGoBack_v2.0c.pdx.zip YouCannotGoBack.pdx

### 

rm -r YouCannotGoBack_c.pdx
mv YouCannotGoBack.pdx YouCannotGoBack_c.pdx

sed 's|#define SCOREBOARD|//#define SCOREBOARD|g' src/game.h --in-place=.bak
rm src/*bak

cp Source/titles/card_i.png Source/titles/card.png 
make clean
make
rm YouCannotGoBack.pdx/titles/*_i.pdi
rm YouCannotGoBack.pdx/titles/*_c.pdi
zip -r -q YouCannotGoBack_v2.0i.pdx.zip YouCannotGoBack.pdx
