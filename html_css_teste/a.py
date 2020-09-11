meuArquivo = open('Config.html', 'r')
txt = meuArquivo.read()
txt = txt.replace('\n', '')
txt = txt.replace('\"', '\\\"')
meuArquivo.close()

meuArquivo = open("ConfigNew.html", "w")
meuArquivo.write(txt)
meuArquivo.close()