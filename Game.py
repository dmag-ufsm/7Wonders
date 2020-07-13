# Graphical Interface for 7 Wonders DMAG implementation
# required libs: tkinter

# Antes de executar esse arquivo, compile o jogo:
# $ mkdir build && cd build && cmake .. && make

# ToDo:
# - Arrumar final da era (fazer pular/jogar automático caso ninguém tenha o efeito de jogar a ultima carta)
# - Arrumar carta em baixo do estagio da maravilha (para wonders de 2 e 4 estagios, no momento so ta certo pra wonders de 3)

from tkinter import *
from tkinter import messagebox
from time import sleep
from threading import Thread
import subprocess
import sys
import json

GAME_ROOT = '.'
IO_DIR = './io' # se tiver executando interface e jogo separados usar './build/io'
BUILD_DIR = './build'

### variables
players = 3
secs_to_load = 0.5 # segundos de espera entre apertar o jogar e testar se ja houve o processamento da rodada
blackwhite_effect = True # se lagar demais pra deixar preto e branco, desativar isso
w_width = 1300
w_height = 705
header_h = 38
pad = 5
bg = '#FFFFFF'
btn_bg = '#CCCCCC'
btn_bg_active = '#449944'
action_active = [0]*players
card_selected = [(-1, '')]*players
root = Tk()
canvas = Canvas(root, bg=bg)
label_infos = [None]
img_clockwise = [None]
img_wonder = [None]*players
img_wonder_stage = [None]*players
img_hand_cards = [None]*players
img_resources = [None]*players
lab_resources_v = [None]*players
img_points = [None]*players
lab_points_v = [None]*players
for i in range(len(lab_points_v)):
    lab_points_v[i] = [None] * 8
img_played_cards = [None]*players
img_buttons = [None]*players
buttons = [None]*players
actions = ['build_structure', 'build_wonder', 'discard', 'build_hand_free']
game_json = {}

def load_image(local, f):
    f_z, f_ss = f
    img = PhotoImage(file=local)
    if f_z != 1:
        img = img.zoom(f_z)
    if f_ss != 1:
        img = img.subsample(f_ss)
    return img

def greyscale(image):
    for x in range(image.width()):
        for y in range(image.height()):
            p = image.get(x, y)
            p = int(p[0]*0.3 + p[1]*0.59 + p[2]*0.11)
            image.put('#%02x%02x%02x' % (p, p, p), (x, y))
    return image

size = {
    'wonder' : (2, 3),
    'wonder_stage' : (2, 3),
    'hand_card' : (1, 4),
    'played_card' : (1, 5),
    'resource' : (1, 4),
    'button' : (1, 1),
    'clockwise' : (1, 1)
}
img = {
    'wonder' : load_image(GAME_ROOT+'/images/wonders/board_0.png', size['wonder']),
    'wonder_stage' : load_image(GAME_ROOT+'/images/wonders/wonder_stage.png', size['wonder_stage']),
    'hand_card' : load_image(GAME_ROOT+'/images/cards/Mine.png', size['hand_card']),
    'played_card' : load_image(GAME_ROOT+'/images/cards/Mine.png', size['played_card']),
    'resource' : load_image(GAME_ROOT+'/images/resources/wood.png', size['resource']),
    'button' : load_image(GAME_ROOT+'/images/resources/discard.png', size['button'])
}

def sort(cards):
    with open(GAME_ROOT+'/references/cards_id.json') as f:
        ids = json.load(f)
    for passnum in range(len(cards)-1,0,-1):
        for i in range(passnum):
            if ids[cards[i]] > ids[cards[i+1]]:
                tmp = cards[i]
                cards[i] = cards[i+1]
                cards[i+1] = tmp
    return cards

def load_wonder():
    global img_wonder
    for i in range(players):
        wonder_id = game_json['players'][str(i)]['wonder_id']
        img_wonder[i] = load_image(GAME_ROOT+'/images/wonders/board_'+str(wonder_id)+'.png', size['wonder'])
        canvas.create_image(pad + (w_width/players)*i, header_h + pad, image=img_wonder[i], anchor=NW)

def load_wonder_stages():
    global img_wonder_stage
    for i in range(players):
        wonder_stage = game_json['players'][str(i)]['wonder_stage']
        img_wonder_stage[i] = [None]*wonder_stage
        for j in range(wonder_stage):
            img_wonder_stage[i][j] = load_image(GAME_ROOT+'/images/wonders/wonder_stage.png', size['wonder_stage'])
            x = pad + (w_width/players)*i + img['wonder'].width()*0.065 + j*(img['wonder_stage'].width() + img['wonder'].width()*0.035)
            y = header_h + pad + img['wonder'].height()
            canvas.create_image(x, y, image=img_wonder_stage[i][j], anchor=NW)

def hand_card_position(player, card):
    x = pad + (w_width/players)*player
    y = header_h + img['wonder'].height() + img['wonder_stage'].height() + pad*(card+2) + img['hand_card'].height()*card
    return (x, y)

def load_resources():
    p = {'wood' : 0, 'stone' : 1, 'ore' : 2, 'clay' : 3, 'papyrus' : 4, 'loom' : 5,
         'glass' : 6, 'coins' : 7, 'compass': -1, 'gear' : -1, 'tablet' : -1, 'shields' : 8}
    for i in range(players):
        resources = game_json['players'][str(i)]['resources']
        img_resources[i] = [None]*len(resources)
        lab_resources_v[i] = [None]*len(resources)
        j = 0
        for r in resources:
            if p[r] != -1:
                img_resources[i][j] = load_image(GAME_ROOT+'/images/resources/' + r + '.png', size['resource'])
                x = pad*2 + (w_width/players)*i + img['hand_card'].width() + (5+img['resource'].width())*p[r]
                y = header_h + pad*2 + img['wonder'].height() + img['wonder_stage'].height()
                canvas.create_image(x, y, image=img_resources[i][j], anchor=NW)

                lab_resources_v[i][j] = Label(canvas, bg=bg, text=resources[r], font=("Verdana", 11))
                x = pad*2 + (w_width/players)*i + img['hand_card'].width() + (5+img['resource'].width())*p[r]
                x = x if resources[r] >= 10 else x + img['resource'].width()*0.15
                y = header_h + pad*2 + img['wonder'].height() + img['wonder_stage'].height() + img['resource'].height()
                lab_resources_v[i][j].place(x=x, y=y)
                j = j + 1
        
def load_vps():
    global lab_points_v
    p = {'civilian' : 1, 'commercial' : 2, 'military' : 3, 'scientific' : 4, 'guild' : 5, 'wonder' : 6, 'total' : 8}

    for i in range(players):
        points = game_json['players'][str(i)]['points']
        img_points[i] = [None]*len(points)
        #lab_points_v[i] = [None] * (len(points)+1)
        j = 0
        for pt in points:
            img_points[i][j] = load_image(GAME_ROOT+'/images/resources/vp_' + pt + '.png', size['resource'])
            x = pad*2 + (w_width/players)*i + img['hand_card'].width() + (5+img['resource'].width())*p[pt]
            y = header_h + pad*2 + img['wonder'].height() + img['wonder_stage'].height() + 2*img['resource'].height()
            canvas.create_image(x, y, image=img_points[i][j], anchor=NW)

            x = pad*2 + (w_width/players)*i + img['hand_card'].width() + (5+img['resource'].width())*p[pt] + img['resource'].width()*0.45 + 1
            y = header_h + pad*2 + img['wonder'].height() + img['wonder_stage'].height() + 2.5*img['resource'].height() - 2
            canvas.delete(lab_points_v[i][j])
            lab_points_v[i][j] = canvas.create_text((x, y), text=points[pt], font=("Verdana", 11))
            j = j + 1
        
        vp_label = Label(canvas, bg=bg, text='VPs', font=("Verdana", 9))
        x = pad*2 + (w_width/players)*i + img['hand_card'].width() + (5+img['resource'].width())*0
        y = header_h + pad*2 + img['wonder'].height() + img['wonder_stage'].height() + 2*img['resource'].height() + 2
        vp_label.place(x=x, y=y)

def load_hand_cards():
    global img_hand_cards
    for i in range(players):
        cards_hand = game_json['players'][str(i)]['cards_hand']
        cards_playable = game_json['players'][str(i)]['cards_playable']
        img_hand_cards[i] = [None]*len(cards_hand)
        for j, c in enumerate(cards_hand):
            img_hand_cards[i][j] = load_image(GAME_ROOT+'/images/cards/' + c + '.png', size['hand_card'])
            x, y = hand_card_position(i, j)
            canvas.create_image(x, y, image=img_hand_cards[i][j] if ((c in cards_playable and action_active[i] == 0) or (action_active[i] != 0) or not blackwhite_effect) else greyscale(img_hand_cards[i][j]), anchor=NW)

def load_played_cards():
    global img_played_cards
    for i in range(players):
        cards_played = sort(game_json['players'][str(i)]['cards_played'])
        img_played_cards[i] = [None]*len(cards_played)
        for j, c in enumerate(cards_played):
            img_played_cards[i][j] = load_image(GAME_ROOT+'/images/cards/' + c + '.png', size['played_card'])
            x = pad*2 + (w_width/players)*i + img['hand_card'].width()
            y = header_h + pad*3 + 3 + img['wonder'].height() + img['wonder_stage'].height() + 3*img['resource'].height() + (j%9)*img['played_card'].height()*0.8
            x = x if j < 9 else x + pad + img['played_card'].width()
            canvas.create_image(x, y, image=img_played_cards[i][j], anchor=NW)
            j = j + 1

def load_header():
    global label_infos, img_clockwise
    text = 'Era: ' + str(game_json['game']['era']) + '     Turn: ' + str(game_json['game']['turn']+1) + '     ' + ('>>' if game_json['game']['clockwise'] else '<<')
    label_infos = Label(canvas, bg=bg, text=text, font=("Verdana", 11))
    label_infos.place(x=pad, y=pad*2)
    btn_play = Button(canvas, width=20, text='Jogar', font=('Verdana', 11), command=play)
    btn_play.place(x=w_width/2-60, y=5)

def change_action(player, action):
    global buttons
    if action_active[player] == action:
        return
    
    # se selecionou build_wonder mas nao possui recursos pra fazer
    if action == 1 and not game_json['players'][str(player)]['can_build_wonder']: # 1 = build_wonder
        messagebox.showinfo('Player '+str(player), 'Not enough resources to build wonder stage')
        return

    update_colors = False
    # se era ou vai ser build_structure colore/descolore as cartas jogaveis, outras acoes deixam todas coloridas/jogaveis
    if action_active[player] == 0 or action == 0: # 0 = build_structure
        update_colors = True

    buttons[player][action_active[player]]['bg'] = btn_bg
    action_active[player] = action
    buttons[player][action]['bg'] = btn_bg_active

    if update_colors:
        # ToDo: esse trecho aparece varias vezes, transformar em funcao pra deixar mais limpo
        cards_hand = game_json['players'][str(player)]['cards_hand']
        cards_playable = game_json['players'][str(player)]['cards_playable']
        img_hand_cards[player] = [None]*len(cards_hand)
        for j, c in enumerate(cards_hand):
            img_hand_cards[player][j] = load_image(GAME_ROOT+'/images/cards/' + c + '.png', size['hand_card'])
            x, y = hand_card_position(player, j)
            canvas.create_image(x, y, image=img_hand_cards[player][j] if ((c in cards_playable and action_active[player] == 0) or (action_active[player] != 0) or not blackwhite_effect) else greyscale(img_hand_cards[player][j]), anchor=NW)        
        card_selected[player] = (-1, '')
    
def load_action_buttons():
    global img_buttons, buttons
    for i in range(players):
        img_buttons[i] = [None]*4
        buttons[i] = [None]*4
        for j in range(len(actions)):
            img_buttons[i][j] = load_image(GAME_ROOT+'/images/resources/' + actions[j] + '.png', size['button'])
            x = pad*3 + (w_width/players)*i + img['wonder'].width()
            y = header_h + pad + j*(img['button'].height() + 2*pad)
            bg_color = btn_bg_active if action_active[i] == j else btn_bg
            width = img['button'].width()*2.5

            if game_json['players'][str(i)]['can_build_hand_free']:
                if actions[j] == 'build_structure':
                    width = width*0.5
                if actions[j] == 'build_hand_free':
                    x = pad*3 + (w_width/players)*i + img['wonder'].width() + width*0.5
                    y = header_h + pad + 0*(img['button'].height() + 2*pad)
                    width = width*0.5
            elif actions[j] == 'build_hand_free':
                continue

            buttons[i][j] = Button(canvas, image=img_buttons[i][j], bg=bg_color, width=width, command=lambda p=i, a=j : change_action(p, a))
            buttons[i][j].place(x=x, y=y, anchor=NW)

def mouse_clicked(event):
    for i in range(players):
        cards_hand = game_json['players'][str(i)]['cards_hand']
        for j in range(len(cards_hand)):
            x, y = hand_card_position(i, j)
            if x < event.x < (x + img['hand_card'].width()) and y < event.y < (y + img['hand_card'].height()):
                # deseleciona a carta que estava selecionada e seleciona a carta clicada
                card_deselected = card_selected[i]
                if card_deselected[0] != -1:
                    cards_playable = game_json['players'][str(i)]['cards_playable']
                    x, y = hand_card_position(i, card_deselected[0])
                    img_hand_cards[i][card_deselected[0]] = load_image(GAME_ROOT+'/images/cards/' + card_deselected[1] + '.png', size['hand_card'])
                    canvas.create_image(x, y, image=img_hand_cards[i][card_deselected[0]] if ((card_deselected[1] in cards_playable and action_active[i] == 0) or (action_active[i] != 0) or not blackwhite_effect) else greyscale(img_hand_cards[i][card_deselected[0]]), anchor=NW)

                card_selected[i] = (j, game_json['players'][str(i)]['cards_hand'][j])
                x, y = hand_card_position(i, j)
                img_hand_cards[i][j] = img_hand_cards[i][j].zoom(7)
                img_hand_cards[i][j] = img_hand_cards[i][j].subsample(8)
                f = (1-7/8)/2
                x = x + img_hand_cards[i][j].width()*f
                y = y + img_hand_cards[i][j].height()*f
                canvas.create_image(x, y, image=img_hand_cards[i][j], anchor=NW)

def new_turn():
    global game_json, label_infos, root

    if game_json['game']['turn'] == 20:
        root.destroy()
        sys.exit()

    with open(IO_DIR+'/game_status.json') as f:
        game_json = json.load(f)

    label_infos['text'] = 'Era: ' + str(game_json['game']['era']) + '     Turn: ' + str(game_json['game']['turn']%7+1) + '     ' + ('>>' if game_json['game']['clockwise'] else '<<')

    for i in range(players):
        card_selected[i] = (-1, '')

    load_action_buttons()
    load_wonder_stages()
    load_hand_cards()
    load_resources()
    load_vps()
    load_played_cards()

def play():
    global bots

    for i in range(players):
        if i < bots:
            continue

        if card_selected[i][0] == -1:
            messagebox.showinfo('Player '+str(i), 'Select a card')
            return
        if action_active[i] == 0 and not card_selected[i][1] in game_json['players'][str(i)]['cards_playable']:
            messagebox.showinfo('Player '+str(i), 'Select a playable card')
            return

    file_ready = open(IO_DIR+'/ready.txt', 'w')
    data = {}
    for i in range(players):
        data['command'] = {
            'subcommand': actions[action_active[i]],
            'argument': card_selected[i][1],
            'extra': ""
        }
        
        if i >= bots:
            with open(IO_DIR+'/player_'+str(i+1)+'.json', 'w') as f:
                json.dump(data, f)

        file_ready.write('ready\n')
    file_ready.close()
    sleep(secs_to_load)
    new_turn()

def initGUI():
    global root, game_json

    root.wm_title('7 Wonders')
    canvas.pack(expand=YES, fill=BOTH)
    canvas.bind("<Button-1>", mouse_clicked)

    with open(IO_DIR+'/game_status.json') as f:
        game_json = json.load(f)
    
    load_header()
    load_wonder()
    load_wonder_stages()
    load_action_buttons()
    load_hand_cards()
    load_resources()
    load_vps()
    load_played_cards()

    root.geometry(str(w_width) + 'x' + str(w_height))
    root.mainloop()

class Game(Thread):
    def __init__ (self, exe_path):
        Thread.__init__(self)
        self.exe_path = exe_path

    def run(self):
        subprocess.call(self.exe_path)

if __name__ == "__main__":
    if len(sys.argv) < 2:   
        print('Execute $ ' + sys.argv[0] + ' <num_bots>')

    global bots
    bots = int(sys.argv[1])

#    if not 'startGame=0' in sys.argv:
    game = Game(BUILD_DIR+'/7Wonders')
    game.start()
   
    sleep(secs_to_load)
    initGUI()
    
