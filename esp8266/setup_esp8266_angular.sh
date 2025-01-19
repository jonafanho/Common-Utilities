cp ../angular/*.json ../esp8266/website
cp ../angular/.gitignore ../esp8266/website
cp ../angular/src/styles.css ../esp8266/website/src
cp ../angular/src/theme.scss ../esp8266/website/src
cp -r ../angular/src/app/component ../esp8266/website/src/app
cp -r ../angular/src/app/service ../esp8266/website/src/app

cd website || exit
npm run build --prod

rm -r ../data
mkdir ../data
cp dist/website/browser/* ../data
