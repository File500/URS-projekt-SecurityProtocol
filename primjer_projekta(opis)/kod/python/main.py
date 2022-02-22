import serial
import PySimpleGUI as sg


# Dictionary that acts as a database for authenticated users
card_ids = {"E45A681EC8": ["Matija", 3]}

# Port that is used for UART communication
serialPort = serial.Serial(port="COM4", baudrate=9600, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)


# Converts card_ids to a list usable for GUI table
def users_to_list():
    values = []
    for key, val in card_ids.items():
        values.append([key] + val)
    return values


# Called when a card is detected by a reader
# If a user is in the database, authentication succeeds, otherwise it fails
def authenticate(user_id=""):
    # Get current privilege level
    curr_priv:int = int(window["-PRIVILEGE-"].DisplayText)

    if contents in card_ids.keys():
        user_priv = card_ids[user_id][1]
        if curr_priv <= user_priv:
            s = "User authenticated: {}".format(card_ids[user_id][0])
            window["-STATUS-"].update(value=s)
            msg = '1'
            serialPort.write(msg.encode())
            return
    # Authentication failed
    s = "Authentication failed!"
    window["-STATUS-"].update(value=s)
    msg = '0'
    serialPort.write(msg.encode())


# Changes privilege level depending on the data that was send from the mcu
def change_privilege(priv):
    curr_priv = int(window["-PRIVILEGE-"].DisplayText)
    if priv[0] == '+':
        if curr_priv < 3:
            window["-PRIVILEGE-"].update(value=(curr_priv+1))
    else:
        if curr_priv > 1:
            window["-PRIVILEGE-"].update(value=(curr_priv-1))


if __name__ == '__main__':
    print('Starting reader app')

    # Create the window and the GUI layout
    headings = ["Id", "Name", "Privilege level"]
    column1 = [
        [
            sg.Text("Status:", font=("Cambria", 20)),
            sg.Text("Waiting...", key="-STATUS-", size=(25, 1), font=("Cambria", 20)),
        ],
        [
            sg.Text("Current privilege level:", font=("Cambria", 16)),
            sg.Text("2", key="-PRIVILEGE-", size=(30, 1), font=("Cambria", 16)),
        ],
        [
            sg.Text("ID:"),
            sg.In(size=(25, 1), enable_events=True, key="-ID FIELD-"),

        ],
        [
            sg.Text("Name:"),
            sg.In(size=(25, 1), enable_events=True, key="-NAME FIELD-"),
        ],
        [
            sg.Text("Privilege level:"),
            sg.Combo(values=[1, 2, 3], default_value=1, readonly=True, size=[2, 1], key="-USER PRIVILEGE-"),
        ],
        [
            sg.Button("Add", enable_events=True, key="-ADD-"),
        ],
        [
            sg.Table(values=users_to_list(),
                     headings=headings,
                     max_col_width=50,
                     # background_color='light blue',
                     auto_size_columns=False,
                     display_row_numbers=False,
                     justification='right',
                     num_rows=4,
                     enable_events=True,
                     #alternating_row_color='darkblue',
                     key="-TABLE-",
                     row_height=30,),
        ],
        [
            sg.Button("Close", enable_events=True, key="-CLOSE-"),
            sg.Button("Remove selected user", enable_events=True, key="-REMOVE-"),

        ],
    ]

    layout = [
        [
            sg.Column(column1),
        ]
    ]
    # Main GUI window
    window = sg.Window("RfidApp", layout, resizable=True, size=(520, 400)).Finalize()

    # Holds data coming from UART
    serialString = ""

    while 1:
        # Wait until there is data waiting in the serial buffer
        if serialPort.inWaiting() > 0:

            # Read data out of the buffer until CR is detected
            serialString = serialPort.readline()

            # Decode contents from the buffer and strip CR
            contents: str = serialString.decode('utf-8').rstrip()

            print(contents)

            # First check if contents aren't empty
            if len(contents) > 0:
                # Char ':' denotes that we're sending card ID
                if contents[0] == ":":
                    contents = contents.strip(":")
                    authenticate(contents)
                    window["-ID FIELD-"].update(value=contents)
                # Char '.' denotes that we're changing privilege level
                elif contents[0] == ".":
                    change_privilege(contents.strip("."))
                else:
                    window["-STATUS-"].update(value=contents)   # Update status in GUI

        # GUI events, updates every 100ms
        event, values = window.read(timeout=100)

        # End program if user closes window or presses the Close button
        if event == "-CLOSE-" or event == sg.WIN_CLOSED:
            break

        # Add new users when Add button is pressed
        if event == "-ADD-":
            card_id: str = values["-ID FIELD-"]
            name: str = values["-NAME FIELD-"]
            privilege: int = values["-USER PRIVILEGE-"]
            if len(card_id) > 6 and len(name) > 0:
                if card_id not in card_ids:
                    user = [name, privilege]
                    card_ids[card_id] = user
                    window["-TABLE-"].Update(users_to_list())
                    window["-NAME FIELD-"].update(value="")

        # Remove users when Remove button is pressed (if user is selected)
        elif event == "-REMOVE-":
            selected_rows = window["-TABLE-"].SelectedRows
            rows = window["-TABLE-"].Values
            if len(selected_rows) > 0:  # if a row is selected
                for i in selected_rows:
                    #index: int = selected_rows[i]
                    uid = rows[i][0]  # get a card id from a selected row
                    card_ids.pop(uid, None)  # remove it from the database
                # update GUI
                window["-TABLE-"].Update(users_to_list())


    print("Shutting down")
    window.close()
    serialPort.close()
