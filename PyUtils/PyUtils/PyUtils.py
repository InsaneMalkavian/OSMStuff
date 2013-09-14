import urllib.request
import subprocess
import os

print('OSM Russia Downloader')
arc7zPath = r'c:\Program Files\7-Zip\7z.exe'
arc7zExtract = 'e'
arc7zNoPrompts = '-y'
urlRootPath = 'http://data.gis-lab.info/osm_dump/dump/latest/'
names = ['RU-AD.osm.bz2', 'RU-AL.osm.bz2', 'RU-ALT.osm.bz2','RU-AMU.osm.bz2']
for name in names:
    print('Downloading ' + name + ' ...')
    urllib.request.urlretrieve(urlRootPath+name, name)
    print('Unpacking ' + name + ' ...')
    subprocess.check_call([arc7zPath, arc7zNoPrompts, arc7zExtract, name])
    os.remove(name)