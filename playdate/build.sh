rm -r YouCannotGoBack.pdx

sed 's|//#define SCOREBOARD|#define SCOREBOARD|g' src/game.h --in-place=.bak
rm src/*bak

cp Source/titles/launchImage_c.png Source/titles/launchImage.png 
cp Source/titles/card_c.png Source/titles/card.png 

make
zip -r -q YouCannotGoBack_v2.0c.pdx.zip YouCannotGoBack.pdx

### 

rm -r YouCannotGoBack.pdx

sed 's|#define SCOREBOARD|//#define SCOREBOARD|g' src/game.h --in-place=.bak
rm src/*bak

cp Source/titles/launchImage_i.png Source/titles/launchImage.png 
cp Source/titles/card_i.png Source/titles/card.png 
make
zip -r -q YouCannotGoBack_v2.0i.pdx.zip YouCannotGoBack.pdx
