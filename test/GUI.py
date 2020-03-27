# Graphical Interface for 7 Wonders DMAG implementation

from tkinter import *
import json

players = 3

root = Tk()
root.wm_title('7 Wonders')

with open('game_status.json') as f:
    game_json = json.load(f)

w_width = 1300
w_height = 665
pad = 5
bg='#FFFFFF'
canvas = Canvas(root, bg=bg)
canvas.pack(expand=YES, fill=BOTH)

def load_image(local, f):
    f_z, f_ss = f
    img = PhotoImage(file=local)
    if f_z != 1:
        img = img.zoom(f_z)
    if f_ss != 1:
        img = img.subsample(f_ss)
    return img

###############
size = {
    'wonder' : (2, 3),
    'wonder_stage' : (2, 3),
    'hand_card' : (1, 4),
    'played_card' : (1, 5),
    'resource' : (1, 4),
    'button' : (1, 1)
}
img = {
    'wonder' : load_image('images/wonders/board_0.png', size['wonder']),
    'wonder_stage' : load_image('images/wonders/wonder_stage.png', size['wonder_stage']),
    'hand_card' : load_image('images/cards/Mine.png', size['hand_card']),
    'played_card' : load_image('images/cards/Mine.png', size['played_card']),
    'resource' : load_image('images/resources/wood.png', size['resource']),
    'button' : load_image('images/resources/discard.png', size['button'])
}

############### WONDERS ###############
img_wonder = [None]*players
img_wonder_stage = [None]*players
for i in range(players):
    wonder_id = game_json['players'][str(i)]['wonder_id']
    wonder_stage = game_json['players'][str(i)]['wonder_stage']
    img_wonder[i] = load_image('images/wonders/board_'+str(wonder_id)+'.png', size['wonder'])
    canvas.create_image(pad + (w_width/players)*i, pad, image=img_wonder[i], anchor=NW)
    img_wonder_stage[i] = [None]*wonder_stage
    for j in range(wonder_stage):
        img_wonder_stage[i][j] = load_image('images/wonders/wonder_stage.png', size['wonder_stage'])
        x = pad + (w_width/players)*i + img['wonder'].width()*0.065 + j*(img['wonder_stage'].width() + img['wonder'].width()*0.035)
        y = pad + img['wonder'].height()
        canvas.create_image(x, y, image=img_wonder_stage[i][j], anchor=NW)
###############

############### HAND CARDS ###############
img_hand_cards = [None]*players
for i in range(players):
    cards_hand = game_json['players'][str(i)]['cards_hand']
    img_hand_cards[i] = [None]*len(cards_hand)
    for j, c in enumerate(cards_hand):
        img_hand_cards[i][j] = load_image('images/cards/' + c + '.png', size['hand_card'])
        x = pad + (w_width/players)*i
        y = img['wonder'].height() + img['wonder_stage'].height() + pad*(j+2) + img['hand_card'].height()*j
        canvas.create_image(x, y, image=img_hand_cards[i][j], anchor=NW)
###############

############# RESOURCES ###############
p = {'wood' : 0, 'stone' : 1, 'ore' : 2, 'clay' : 3, 'papyrus' : 4, 'loom' : 5,
     'glass' : 6, 'coins' : 7, 'compass': -1, 'gear' : -1, 'tablet' : -1, 'shields' : -1}
img_resources = [None]*players
img_value = [None]*players
for i in range(players):
    resources = game_json['players'][str(i)]['resources']
    img_resources[i] = [None]*len(resources)
    j = 0
    img_value[i] = [None]*len(resources)
    for r in resources:
        if p[r] == -1:
            continue
        img_resources[i][j] = load_image('images/resources/' + r + '.png', size['resource'])
        x = pad*5 + (w_width/players)*i + img['hand_card'].width() + (5+img['resource'].width())*p[r]
        y = pad*2 + img['wonder'].height() + img['wonder_stage'].height()
        canvas.create_image(x, y, image=img_resources[i][j], anchor=NW)

        img_value[i][j] = Label(canvas, bg=bg, text=resources[r], font=("Verdana", 11))
        x = pad*5 + (w_width/players)*i + img['hand_card'].width() + (5+img['resource'].width())*p[r]
        x = x if resources[r] >= 10 else x + img['resource'].width()*0.15
        y = pad*2 + img['wonder'].height() + img['wonder_stage'].height() + img['resource'].height()
        img_value[i][j].place(x=x, y=y)
        j = j + 1
###############

############# PLAYED CARDS ###############
img_played_cards = [None]*players
for i in range(players):
    cards_played = game_json['players'][str(i)]['cards_played']
    img_played_cards[i] = [None]*len(cards_played)
    for j, c in enumerate(cards_played):
        img_played_cards[i][j] = load_image('images/cards/' + c + '.png', size['played_card'])
        x = pad*2 + (w_width/players)*i + img['hand_card'].width()
        y = pad*3 + 3 + img['wonder'].height() + img['wonder_stage'].height() + 2*img['resource'].height() + (j%9)*img['played_card'].height()*0.9
        x = x if j < 9 else x + pad + img['played_card'].width()
        canvas.create_image(x, y, image=img_played_cards[i][j], anchor=NW)
        j = j + 1
###############

############### BUTTONS ###############
button_active = [0]*players
card_selected = ['']*players

def build_structure():
    print(str(0)+' apertou build_structure')
    #myLabel = Label(canvas, text="Card!")
    #myLabel.place(x=500, y=500)

def build_wonder(player):
    print(str(player)+' apertou build_wonder')
    #myLabel = Label(canvas, text="Wonder!")
    #myLabel.place(x=500, y=500)

def discard(player):
    print(str(player)+' apertou discard')
    #myLabel = Label(canvas, text="Coins!")
    #myLabel.place(x=500, y=500)

actions = ['build_structure', 'build_wonder', 'discard']
img_buttons = [None]*players
for i in range(players):
    img_buttons[i] = [None]*3
    for j in range(len(actions)):
        img_buttons[i][j] = load_image('images/resources/' + actions[j] + '.png', size['button'])
        x = pad*3 + (w_width/players)*i + img['wonder'].width()
        y = pad + j*(img['button'].height() + 2*pad)
        bg_color = '#449944' if button_active[i] == j else '#CCCCCC'
        card_button = Button(canvas, image=img_buttons[i][j], bg=bg_color, width=img['button'].width()*2.5, command=build_structure)
        card_button.place(x=x, y=y, anchor=NW)
###############

root.geometry(str(w_width) + 'x' + str(w_height))
root.mainloop()
