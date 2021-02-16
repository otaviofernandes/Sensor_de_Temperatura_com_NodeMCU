# Sensor_de_Temperatura_com_NodeMCU

Projeto de sensoriamento de temperatura utilizando o Módulo WiFi ESP8266 NodeMcu ESP-12.
Trata-se de um projeto desenvolvido com a placa ESP8266 NodeMcu ESP-12, conectado ao módulo leitor de temperatura Max6675 e um termopar tipo K (faxa de leitura de 0°C a 800°C, com precisão de 1,5 °C). A placa do NodeMCU está configurada para trabalhar como webserver, disponibilizando as leituras de temperatura para acesso via internet. Assim, os dados do NodeMCU são acessados via requisição TCP/IP pelo programa escrito em python 3 (versão 3.8), e plotados em um gráfico dinâmico, criado a partir da biblioteca matplotlib.
