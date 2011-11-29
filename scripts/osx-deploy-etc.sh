rm -rfv "Learn Greenlandic 2.app"
time macdeployqt LearnGreenlandic2.app -dmg
mv LearnGreenlandic2.app "Learn Greenlandic 2.app"
rsync -avzP --delete "Learn Greenlandic 2.app" kal@learngreenlandic.com:files/demo2/
