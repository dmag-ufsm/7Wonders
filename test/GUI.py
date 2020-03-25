from tkinter import *
import json

players = 3

root = Tk()
root.wm_title('7 Wonders')

with open('game_status.json') as f:
    game_json = json.load(f)

w_width = 1300
w_height = 665
canvas = Canvas(root, width=w_width, height=w_height, bg='white')
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
    'hand_cards' : (1, 4),
    'played_cards' : (1, 5),
    'resources' : (1, 4)
}
img = {
    'wonder' : load_image('images/wonders/board_0.png', size['wonder']),
    'wonder_stage' : load_image('images/wonders/wonder_stage.png', size['wonder_stage']),
    'hand_cards' : load_image('images/cards/Mine.png', size['hand_cards']),
    'played_cards' : load_image('images/cards/Mine.png', size['played_cards']),
    'resources' : load_image('images/resources/wood.png', size['resources'])
}

############### WONDERS ###############
img_wonder = [None]*players
img_wonder_stage = [None]*players
for i in range(players):
    wonder_id = game_json['players'][str(i)]['wonder_id']
    wonder_stage = game_json['players'][str(i)]['wonder_stage']
    img_wonder[i] = load_image('images/wonders/board_'+str(wonder_id)+'.png', size['wonder'])
    canvas.create_image(5 + (w_width/players)*i, 5, image=img_wonder[i], anchor=NW)
    img_wonder_stage[i] = [None]*wonder_stage
    for j in range(wonder_stage):
        img_wonder_stage[i][j] = load_image('images/wonders/wonder_stage.png', size['wonder_stage'])
        x = 5 + (w_width/players)*i + img['wonder'].width()*0.065 + j*(img['wonder_stage'].width() + img['wonder'].width()*0.035)
        y = 5 + img['wonder'].height()
        canvas.create_image(x, y, image=img_wonder_stage[i][j], anchor=NW)
###############

############### HAND CARDS ###############
img_hand_cards = [None]*players
for i in range(players):
    cards_hand = game_json['players'][str(i)]['cards_hand']
    img_hand_cards[i] = [None]*len(cards_hand)
    for j, c in enumerate(cards_hand):
        img_hand_cards[i][j] = load_image('images/cards/' + c + '.png', size['hand_cards'])
        x = 5 + (w_width/players)*i
        y = img['wonder'].height() + img['wonder_stage'].height() + 5*(j+2) + img['hand_cards'].height()*j
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
        n = resources[r] if resources[r] < 10 else 10
        img_resources[i][j] = load_image('images/resources/' + r + '.png', size['resources'])
        x = 10 + (w_width/players)*i + img['hand_cards'].width() + (5+img['resources'].width())*p[r]
        y = 10 + img['wonder'].height() + img['wonder_stage'].height()
        canvas.create_image(x, y, image=img_resources[i][j], anchor=NW)
        img_value[i][j] = load_image('images/resources/' + str(n) + '.png', size['resources'])
        x = 10 + (w_width/players)*i + img['hand_cards'].width() + (5+img['resources'].width())*p[r]
        y = 10 + img['wonder'].height() + img['wonder_stage'].height() + img['resources'].height()
        canvas.create_image(x, y, image=img_value[i][j], anchor=NW)
        j = j + 1
###############

############# PLAYED CARDS ###############
img_played_cards = [None]*players
for i in range(players):
    cards_played = game_json['players'][str(i)]['cards_played']
    img_played_cards[i] = [None]*len(cards_played)
    for j, c in enumerate(cards_played):
        img_played_cards[i][j] = load_image('images/cards/' + c + '.png', size['played_cards'])
        x = 10 + (w_width/players)*i + img['hand_cards'].width()
        y = 18 + img['wonder'].height() + img['wonder_stage'].height() + 2*img['resources'].height() + (j%9)*img['played_cards'].height()*0.9
        if j >= 9:
            x = x + 5 + img['played_cards'].width()
        canvas.create_image(x, y, image=img_played_cards[i][j], anchor=NW)
        j = j + 1
###############

root.mainloop()
