import urllib.request
import subprocess
import os

print('OSM Russia Downloader')
arc7zPath = r'c:\Program Files\7-Zip\7z.exe'
arc7zExtract = 'e'
arc7zNoPrompts = '-y'
urlRootPath = 'http://data.gis-lab.info/osm_dump/dump/latest/'
names = ['RU-ARK.osm.bz2', 'RU-AST.osm.bz2', 'RU-BEL.osm.bz2', 'RU-BRY.osm.bz2', 'RU-CU.osm.bz2', 'RU-KIR.osm.bz2', 'RU-NIZ.osm.bz2', 'RU-VGG.osm.bz2', 'RU-VLA.osm.bz2', 'RU-VLG.osm.bz2']
for name in names:
    print('Downloading ' + name + ' ...')
    urllib.request.urlretrieve(urlRootPath+name, name)
    print('Unpacking ' + name + ' ...')
    subprocess.check_call([arc7zPath, arc7zNoPrompts, arc7zExtract, name])
    os.remove(name)