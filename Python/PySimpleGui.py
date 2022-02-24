import PySimpleGUI as sg
import keyboard

sg.theme('DarkGrey12')

layout = [  [sg.Text('Stopwatch', size =(20, 2), justification ='center')], 
            [sg.Text(size =(10, 2), font =('Arial', 20), justification ='center', key ='-OUTPUT-')], 
            [sg.T(' ' * 5), sg.Button('Start / Stop', focus = True), sg.Quit()]] 
  
window = sg.Window('Stopwatch', layout, resizable=True, finalize=True)
window.bind('<Configure>', "Configure")
status = window['Status']
  
timer_running, counter = True, 0
  
while True:   
  
    
    event, values = window.read(timeout = 10)  
  
    
    if event in (None, 'Quit'):              
        break
    elif event == 'Configure':
        if window.TKroot.state() == 'zoomed':
            status.update(value='Window zoomed and maximized !')
        else:
            status.update(value='Window normal')
    elif event == 'Start / Stop': 
        timer_running = not timer_running 
    if timer_running: 
        window['-OUTPUT-'].update('{:02d}:{:02d}.{:02d}'.format((counter // 100) // 60,  
                                                 (counter // 100) % 60, counter % 100)) 
        counter += 1
window.close() 