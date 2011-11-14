time macdeployqt LearnGreenlandic.app -dmg
mv LearnGreenlandic.app "Learn Greenlandic.app"
rsync -avzP --delete "Learn Greenlandic.app" kal@learngreenlandic.com:public_html/demo/
