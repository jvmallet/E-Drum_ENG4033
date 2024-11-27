import serial
import time

# Configurar a porta serial e o baudrate
porta_serial = "COM3"  # Substitua por sua porta (no Windows geralmente "COMx", no Linux/Mac: "/dev/ttyUSBx")
baud_rate = 9600       # Mesmo baudrate configurado no Arduino

try:
    # Inicializa a comunicação serial
    arduino = serial.Serial(porta_serial, baud_rate, timeout=1)
    time.sleep(2)  # Aguarda inicialização do Arduino

    # Vetores de tempo para os LEDs
    ligado1 = "100,200,300"      # Tempos em milissegundos para a tira 1
    desligado1 = "50,100,150"    # Tempos desligados para a tira 1
    ligado2 = "200,400,600"      # Tempos em milissegundos para a tira 2
    desligado2 = "100,200,300"   # Tempos desligados para a tira 2

    # Formatar a string para enviar
    mensagem = f"L1:{ligado1};{desligado1};{ligado2};{desligado2}\n"
    print(f"Enviando: {mensagem.strip()}")

    # Enviar a mensagem via serial
    arduino.write(mensagem.encode('utf-8'))

    # Aguardar resposta do Arduino
    while arduino.in_waiting == 0:  # Espera resposta do Arduino
        pass

    resposta = arduino.readline().decode('utf-8').strip()
    print(f"Resposta do Arduino: {resposta}")

except serial.SerialException as e:
    print(f"Erro ao se conectar à porta {porta_serial}: {e}")
finally:
    if 'arduino' in locals() and arduino.is_open:
        arduino.close()
        print("Conexão com Arduino encerrada.")
