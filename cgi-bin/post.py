import requests

url = 'http://localhost:8081/upload'
myobj = {'somekey': 'somevalue'}

x = requests.post(url, json = myobj)

print(x.text)

