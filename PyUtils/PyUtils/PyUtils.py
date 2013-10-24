import urllib.request
import subprocess
import os
import json
import codecs
from pprint import pprint

def uploadToFtp(regions):
    for region in regions:
        lines.append(region["osm"]+'.html ')
    lines.append('index.html')
    subprocess.check_call([batUploader, lines])

arc7zPath = r'c:\Program Files\7-Zip\7z.exe'
arc7zExtract = 'e'
arc7zNoPrompts = '-y'
urlRootPath = 'http://data.gis-lab.info/osm_dump/dump/latest/'
exeFuelFilter = r'OSMProcessor.exe'
exeFuelStation = r'OSMFuelStation.exe'
batUploader = r'FtpUploader.bat'

print('OSM Russia Downloader')
regions = json.load(codecs.open('regions.cfg', 'r', 'utf-8-sig'))
lines = []

for region in regions:
    name = region["osm"]+'.osm.bz2'
    nameOSM = region["osm"]+'.osm'
    print(region["name"])
    print('Downloading ' + region["name"] + ' ...')
    urllib.request.urlretrieve(urlRootPath+name, name)
    print('Unpacking ' + name + ' ...')
    subprocess.check_call([arc7zPath, arc7zNoPrompts, arc7zExtract, name])
    os.remove(name)
    subprocess.check_call([exeFuelFilter, region["osm"]])
    os.remove(nameOSM)    

subprocess.check_call([exeFuelStation])
uploadToFtp(regions)