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
action_active = [0]*players
card_selected = [(-1, '')]*players

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

def greyscale(image):
#    return image
    for x in range(image.width()):
        for y in range(image.height()):
            p = image.get(x, y)
            p = int(p[0]*0.3 + p[1]*0.59 + p[2]*0.11)
            image.put('#%02x%02x%02x' % (p, p, p), (x, y))
    return image

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
def card_position(player, card):
    x = pad + (w_width/players)*player
    y = img['wonder'].height() + img['wonder_stage'].height() + pad*(card+2) + img['hand_card'].height()*card
    return (x, y)

img_hand_cards = [None]*players
for i in range(players):
    cards_hand = game_json['players'][str(i)]['cards_hand']
    cards_playable = game_json['players'][str(i)]['cards_playable']
    img_hand_cards[i] = [None]*len(cards_hand)
    for j, c in enumerate(cards_hand):
        img_hand_cards[i][j] = load_image('images/cards/' + c + '.png', size['hand_card'])
        x, y = card_position(i, j)
        canvas.create_image(x, y, image=img_hand_cards[i][j] if (c in cards_playable) else greyscale(img_hand_cards[i][j]), anchor=NW)

def select_card(player, card):
    card_deselected = card_selected[player]
    if card_deselected[0] != -1:
        cards_playable = game_json['players'][str(player)]['cards_playable']
        x, y = card_position(player, card_deselected[0])
        img_hand_cards[player][card_deselected[0]] = load_image('images/cards/' + card_deselected[1] + '.png', size['hand_card'])
        canvas.create_image(x, y, image=img_hand_cards[player][card_deselected[0]] if (card_deselected[1] in cards_playable) else greyscale(img_hand_cards[player][card_deselected[0]]), anchor=NW)

    card_selected[player] = (card, game_json['players'][str(player)]['cards_hand'][card])
    x, y = card_position(player, card)
    img_hand_cards[player][card] = img_hand_cards[player][card].zoom(7)
    img_hand_cards[player][card] = img_hand_cards[player][card].subsample(8)
    f = (1-7/8)/2
    x = x + img_hand_cards[player][card].width()*f
    y = y + img_hand_cards[player][card].height()*f
    canvas.create_image(x, y, image=img_hand_cards[player][card], anchor=NW)

def mouse_clicked(event):
    for i in range(players):
        for j in range(len(cards_hand)):
            x, y = card_position(i, j)
            if x < event.x < (x + img['hand_card'].width()) and y < event.y < (y + img['hand_card'].height()):
                select_card(i, j)
                return

canvas.bind("<Button-1>", mouse_clicked)
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
actions = ['build_structure', 'build_wonder', 'discard']
img_buttons = [None]*players
button = [None]*players
btn_bg = '#CCCCCC'
btn_bg_active = '#449944'
for i in range(players):
    img_buttons[i] = [None]*3
    button[i] = [None]*3
    for j in range(len(actions)):
        img_buttons[i][j] = load_image('images/resources/' + actions[j] + '.png', size['button'])
        x = pad*3 + (w_width/players)*i + img['wonder'].width()
        y = pad + j*(img['button'].height() + 2*pad)
        bg_color = btn_bg_active if action_active[i] == j else btn_bg
        button[i][j] = Button(canvas, image=img_buttons[i][j], bg=bg_color, width=img['button'].width()*2.5, command=lambda p=i, a=j : action(p, a))
        button[i][j].place(x=x, y=y, anchor=NW)

def action(player, action):
    button[player][action_active[player]]['bg'] = btn_bg
    action_active[player] = action
    button[player][action]['bg'] = btn_bg_active

# JOGAR
def play():
    print('Jogando...')
    for i in range(players):
        print('Player', i, actions[action_active[i]], card_selected[i][1])

btn_play = Button(canvas, width=20, text='Jogar', font=('Verdana', 11), command=play)
btn_play.place(x=w_width/2-60, y=5)

###############

root.geometry(str(w_width) + 'x' + str(w_height))
root.mainloop()
