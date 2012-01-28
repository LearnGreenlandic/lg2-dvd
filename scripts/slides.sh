cat | perl -e 'for ($i=1 ; <> ; $i++) { ($m,$s) = m/([0-9]+):([0-9]+)/; $c = sprintf("mv -v D\\ -\\ %02d.png %d.png\n", $i, $m*60+$s); print `$c`; }'
