import requests
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from itertools import count

tempo = [0]
temperatura = [0]
index = count()

def atualiza_dados():
    resposta = requests.get('http://***.***.*.**/') # inserir aqui o endereço ip da placa do NodeMCU.
    dados = resposta.text
    tempo_seg, temperatura_c = dados.split()
    tempo.append(float(tempo_seg)/60)
    temperatura.append(float(temperatura_c))

def grafico_dinamico(i):
    atualiza_dados()
    plt.plot(tempo, temperatura, c='red', ls=':', label='Temp, °C')
    plt.title('Temperatura (°C) por tempo (minutos)', color='black')
    plt.xlabel('Tempo (minutos)')
    plt.ylabel('Temperatura (°C)')

ani = FuncAnimation(plt.gcf(), grafico_dinamico, interval=5000)
plt.tight_layout()
plt.show()


