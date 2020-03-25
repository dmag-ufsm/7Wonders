from tkinter import *
import json

w_width = 1250
w_height = 750

players = 3
res = w_width / 1250

root = Tk()
root.wm_title('7 Wonders')

with open('game_status.json') as f:
    game_json = json.load(f)

canvas = Canvas(root, width=w_width, height=w_height, bg='white')
canvas.pack(expand=YES, fill=BOTH)

def img_resize(img, n):
    prec = 10
    img = img.zoom(int(n * prec))
    img = img.subsample(prec)
    return img

############### WONDERS ###############
img_wonder = [None]*players
img_wonder_stage = [None]*players
for i in range(players):
    wonder_id = game_json['players'][str(i)]['wonder_id']
    wonder_stage = game_json['players'][str(i)]['wonder_stage']
    img_wonder[i] = PhotoImage(file='images/wonders/board_'+str(wonder_id)+'.png')
    img_wonder[i] = img_resize(img_wonder[i], 0.6*res)
    canvas.create_image(5*res + (w_width/players)*i, 5*res, image=img_wonder[i], anchor=NW)
    img_wonder_stage[i] = [None]*wonder_stage
    for j in range(wonder_stage):
        img_wonder_stage[i][j] = PhotoImage(file='images/wonders/wonder_stage.png')
        img_wonder_stage[i][j] = img_resize(img_wonder_stage[i][j], 0.6*res)
        x = 5*res + (w_width/players)*i + img_wonder[0].width()*0.065 + j*(img_wonder_stage[0][0].width() + img_wonder[0].width()*0.035)
        y = 5*res + img_wonder[0].height()
        canvas.create_image(x, y, image=img_wonder_stage[i][j], anchor=NW)
###############

############### HAND CARDS ###############
img_hand_cards = [None]*players
for i in range(players):
    cards_hand = game_json['players'][str(i)]['cards_hand']
    img_hand_cards[i] = [None]*len(cards_hand)
    for j, c in enumerate(cards_hand):
        img_hand_cards[i][j] = PhotoImage(file='images/cards/' + c + '.png')
        img_hand_cards[i][j] = img_resize(img_hand_cards[i][j], 0.2*res)
        x = 5*res + (w_width/players)*i
        y = img_wonder[0].height() + img_wonder_stage[0][0].height() + 5*(j+2)*res + img_hand_cards[0][0].height()*j
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
        img_resources[i][j] = PhotoImage(file='images/resources/' + r + '.png')
        img_resources[i][j] = img_resize(img_resources[i][j], 0.30*res)
        x = 10*res + (w_width/players)*i + img_hand_cards[0][0].width() + (5*res+img_resources[0][0].width())*p[r]
        y = 10*res + img_wonder[0].height() + img_wonder_stage[0][0].height()
        canvas.create_image(x, y, image=img_resources[i][j], anchor=NW)
        img_value[i][j] = PhotoImage(file='images/resources/' + str(n) + '.png')
        img_value[i][j] = img_resize(img_value[i][j], 0.30*res)
        x = 10*res + (w_width/players)*i + img_hand_cards[0][0].width() + (5*res+img_resources[0][0].width())*p[r]
        y = 10*res + img_wonder[0].height() + img_wonder_stage[0][0].height() + img_resources[0][0].height()
        canvas.create_image(x, y, image=img_value[i][j], anchor=NW)
        j = j + 1
###############

############# PLAYED CARDS ###############

###############

root.mainloop()
