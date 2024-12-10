import time
import mido
from mido import Message
import pygame
from pygame import mixer
from serial import Serial
import threading

serial = Serial("COM18", baudrate=31250)

pygame.init()

# Colors
black = (0, 0, 0)
white = (255, 255, 255)
gray = (128, 128, 128)
dark_gray = (50, 50, 50)
light_gray = (170, 170, 170)
blue = (0, 255, 255)
red = (255, 0, 0)
green = (0, 255, 0)
purple = (128, 0, 128)
gold = (212, 175, 55)

# Screen settings
WIDTH = 1400
HEIGHT = 800
active_length = 0
active_beat = 0
volume = 80
fps = 60
bpm = 240
beat_length = 3600 // bpm

timer = pygame.time.Clock()

# Variables
beats = 8
instruments = 6
playing = True
beat_changed = True
clicked = [[-1 for _ in range(beats)] for _ in range(instruments)]
active_list = [1 for _ in range(instruments)]
current_led = None
waiting_for_input = False
start_time = None
response_time_limit = 2  # Time limit in seconds

# MIDI setup
port = mido.open_output('loopMIDI Port 1')

def nota(nota, velocity):
    print("nota tocada, velocidade = ", velocity)
    port.send(Message('note_on', note=nota, velocity=velocity))

def send_led_command(led, state):
    # Send command to Arduino: 'led 1 purple', 'led 1 green', 'led 1 red', etc.
    command = f"led {led} {state}\n"
    serial.write(command.encode("UTF-8"))
    print(f"Comando enviado: {command.strip()}")

def draw_grid():
    boxes = []
    for i in range(beats):
        for j in range(instruments):
            color = green if clicked[j][i] == 1 else gray
            rect = pygame.draw.rect(
                screen, color, [i * ((WIDTH - 200) // beats) + 205, (j * 100) + 5, ((WIDTH - 200) // beats) - 10, 90]
            )
            pygame.draw.rect(
                screen, gold, [i * ((WIDTH - 200) // beats) + 200, j * 100, ((WIDTH - 200) // beats), 100], 5
            )
            boxes.append((rect, (i, j)))
    return boxes

def play_notes():
    global current_led, waiting_for_input, start_time
    for i in range(len(clicked)):
        if clicked[i][active_beat] == 1 and active_list[i] == 1:
            current_led = i + 1
            send_led_command(current_led, "purple")  # Set LED to purple initially
            start_time = time.time()
            waiting_for_input = True
            return

def check_input():
    global waiting_for_input, playing
    if waiting_for_input:
        elapsed_time = time.time() - start_time
        if elapsed_time > response_time_limit:
            send_led_command(current_led, "red")  # Turn LED red on timeout
            print("Jogador errou! Fim do jogo.")
            playing = False
            waiting_for_input = False
        else:
            # Check for user input via serial (e.g., 'acerto' or 'erro')
            if serial.in_waiting > 0:
                response = serial.readline().decode().strip()
                if response == "acerto":
                    send_led_command(current_led, "green")  # Turn LED green on success
                    waiting_for_input = False
                elif response == "erro":
                    send_led_command(current_led, "red")  # Turn LED red on failure
                    playing = False
                    waiting_for_input = False

def main():
    global active_beat, beat_changed, playing

    screen = pygame.display.set_mode([WIDTH, HEIGHT])
    pygame.display.set_caption('The Beat Maker')

    run = True
    while run:
        timer.tick(fps)
        screen.fill(black)
        draw_grid()

        if beat_changed and playing:
            play_notes()
            beat_changed = False

        check_input()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False

        if playing:
            if active_length < beat_length:
                active_length += 1
            else:
                active_length = 0
                if active_beat < beats - 1:
                    active_beat += 1
                    beat_changed = True
                else:
                    active_beat = 0
                    beat_changed = True

        pygame.display.flip()

    pygame.quit()

if __name__ == "__main__":
    main()
