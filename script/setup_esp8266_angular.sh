cp ../angular/*.json ../esp8266/website
cp ../angular/.gitignore ../esp8266/website
cp ../angular/src/styles.css ../esp8266/website/src
cp ../angular/src/theme.scss ../esp8266/website/src
cp ../angular/src/app/service/theme.service.ts ../esp8266/website/src/app/service

cd ../esp8266/website || exit
npm run build

rm -r ../data
mkdir ../data
cp dist/website/browser/* ../data
mv ../data/index.html ../data/setup.html
