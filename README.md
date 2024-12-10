# The Beat Maker - Projeto de Simulação de Bateria

## Descrição
The Beat Maker é um projeto de simulação de bateria que utiliza o **Pygame** para criar uma interface gráfica, o **Arduino** para controle de LEDs e sensores, e o **MIDI** para produzir sons. O jogador precisa acertar as batidas dentro do tempo limite para manter o jogo ativo. Este projeto combina elementos de música, lógica de jogos e hardware interativo.

## Requisitos
### Software Necessário
1. **Python 3.12 ou superior**
2. **Bibliotecas Python**:
   - Pygame
   - Pyserial
   - Mido
   - Python-rtmidi

   Instale as bibliotecas executando:
   ```bash
   pip install pygame pyserial mido python-rtmidi
   ```

3. **Roboto Bold Font**:
   - Baixe o arquivo de fonte `Roboto-Bold.ttf` e coloque-o no mesmo diretório do projeto.
   - [Baixar Roboto Bold](https://fonts.google.com/specimen/Roboto).

4. **LoopMIDI**:
   - Baixe e instale o programa LoopMIDI: [https://www.tobias-erichsen.de/software/loopmidi.html](https://www.tobias-erichsen.de/software/loopmidi.html)
   - Após instalar:
     1. Abra o programa.
     2. Clique no botão `+` sem mudar o nome.

5. **VSCode**:
   - Recomendamos usar o Visual Studio Code para executar o projeto, pois outros IDEs podem ter problemas com os imports.

### Hardware Necessário
1. **Arduino**:
   - Modelo recomendado: Arduino Uno.
2. **Fitas de LED (WS2812B)**:
   - Conecte as fitas aos pinos especificados no código do Arduino.
3. **Sensores Analógicos**:
   - Conecte sensores aos pinos A0, A1, A2 e A3 no Arduino.

## Configuração do Projeto
1. **Clone o repositório**:
   ```bash
   git clone <url-do-repositorio>
   cd <nome-do-diretorio>
   ```

2. **Conecte o Arduino**:
   - Compile e envie o código `leds.ino` para o Arduino usando a IDE do Arduino.

3. **Configuração MIDI**:
   - Abra o LoopMIDI.
   - Certifique-se de que uma porta MIDI virtual está criada.

4. **Execute o jogo**:
   - Abra o terminal no diretório do projeto.
   - Execute:
     ```bash
     python interface.py
     ```

## Tutorial de Uso
1. **Interface do Jogo**:
   - A interface gráfica mostra uma grade de batidas.
   - Cada linha representa um instrumento (Hi Hat, Snare, etc.).
   - O jogador precisa acertar o sensor correspondente quando a luz roxa estiver acesa.

2. **Cores dos LEDs**:
   - **Roxo**: Indica o LED ativo que o jogador precisa acertar.
   - **Verde**: Indica que o jogador acertou.
   - **Vermelho**: Indica que o jogador errou.

3. **Controles no Pygame**:
   - Configure o número de batidas por loop e o BPM (batidas por minuto) diretamente na interface gráfica.
   - Use o botão "Play/Pause" para iniciar ou pausar o jogo.

## Fluxo de Funcionamento
1. **Python controla o fluxo**:
   - Envia comandos ao Arduino para acender LEDs e mudar suas cores.
   - Recebe respostas do Arduino (`acerto` ou `erro`).

2. **Arduino gerencia LEDs e sensores**:
   - LEDs são acesos em cores específicas de acordo com o comando do Python.
   - Monitora os sensores para detectar se o jogador acertou a batida correta dentro do tempo limite.

3. **Som MIDI**:
   - O Python envia notas MIDI para simular os sons dos instrumentos.
   - Use o LoopMIDI para redirecionar as notas para um sintetizador MIDI.

## Solução de Problemas
1. **Erro com fonte Roboto-Bold.ttf**:
   - Verifique se o arquivo `Roboto-Bold.ttf` está na mesma pasta do projeto.

2. **Erro de importação**:
   - Certifique-se de instalar as dependências com o comando:
     ```bash
     pip install pygame pyserial mido python-rtmidi
     ```

3. **Som MIDI não funciona**:
   - Certifique-se de que o LoopMIDI está aberto e a porta MIDI está configurada corretamente.

4. **Serial não funciona**:
   - Certifique-se de usar a biblioteca `pyserial` e não `serial`.

5. **Administração de downloads**:
   - Alguns computadores podem exigir permissões administrativas para instalar programas como LoopMIDI. Consulte o administrador da máquina se necessário.



