import requests
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from itertools import count
import PySimpleGUI as sg


def testa_arquivo():
    try:
        arquivo = open('ipsalvo.txt', 'r')
    except FileNotFoundError:
        arquivo = open('ipsalvo.txt', 'w')
        arquivo.close()


def le_ipsalvo():
    arquivo = open('ipsalvo.txt', 'r')
    ipsalvo = arquivo.read()
    arquivo.close
    return ipsalvo


def salva_ultimo_ip(ip):
    arquivo = open('ipsalvo.txt', 'w')
    arquivo.write(ip)
    arquivo.close()


def escala():
    sg.theme('DefaultNoMoreNagging')
    layout = [
        [sg.Text('Escolha a escala para leitura de temperatura')],
        [sg.Radio('Celsius', escala, key='c'), sg.Radio('Fahrenheit', escala, key='f')],
        [sg.Text('')],
        [sg.Text('Digite o endereço IP para conexão com o NodeMCU')],
        [sg.Text('IP do último acesso -> ' + ipsalvo)],
        [sg.Input()],
        [sg.Button('Iniciar ou Finalizar')]
    ]
    return sg.Window('Escala de Temperatura', layout=layout, finalize=True)


def atualiza_dados(tp, ip):
    resposta = requests.get('http://'+ip+'/')
    dados = resposta.text
    tempo_seg, temperatura_c = dados.split()
    tempo.append(float(tempo_seg)/60)
    if tp =='C':
        temperatura.append(float(temperatura_c))
    else:
        temperatura_c = float(temperatura_c)*9/5 +32
        temperatura.append(temperatura_c)


def grafico_dinamico(i):
    atualiza_dados(tp, ip)
    if tp == 'C':
        plt.plot(tempo, temperatura, c='red', ls=':', label='Temp, °C')
        plt.title('Temperatura (°C) por tempo (minutos)', color='black')
        plt.xlabel('Tempo (minutos)')
        plt.ylabel('Temperatura (°C)')
    else:
        plt.plot(tempo, temperatura, c='red', ls=':', label='Temp, °F')
        plt.title('Temperatura (°F) por tempo (minutos)', color='black')
        plt.xlabel('Tempo (minutos)')
        plt.ylabel('Temperatura (°F)')

# ----------------------------------------------------------------------------------
tempo = [0]
temperatura = [0]
index = count()

testa_arquivo()
ipsalvo = le_ipsalvo()

janela = escala()
janela.maximize()

event, values = janela.Read()
if event == sg.WIN_CLOSED:
    exit(69)

ip = values[0]
salva_ultimo_ip(ip)

if values['c']:
    tp = 'C'
else:
    tp = 'F'
ani = FuncAnimation(plt.gcf(), grafico_dinamico, interval=1000)
plt.show()
