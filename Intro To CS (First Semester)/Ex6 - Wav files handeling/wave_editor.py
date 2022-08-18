import wave_helper
import math
import copy
import os.path

MODIFY_WAV = "1"  # enter 1 to  to modify the wav file
COMPOSE_WAV = "2"  # enter 2 to compose wav file
EXIT = "3"  # enter 3 to exit
# modify wav options
REVERSE = "1"  # enter 1 to reverse the Audio file
NEGATIVE_AUDIO = "2"  # enter 2 to negative Audio file
ACCELERATION = "3"  # enter 3 to accelerate the Audio file
SLOW_DOWN = "4"  # enter 4 to slow down the Audio file
INCREASING_VOLUME = "5"  # enter 5 to volume up
DECREASING_VOLUME = "6"  # enter 6 to  volume down
LOW_PASS_FILTER = "7"  # enter 7 to low pass filter
EXIT_MODIFY = "8"  # enter 8 to exit the modify menu
MAX_VOLUME = 32767


def gen_input():
    """this function prints the main menu, and asks  the user  for his choice,
    if the input is wrong it returns error,if the input is right
    in returns the users choice"""
    print("Choose action:")
    print(MODIFY_WAV + " : for modifying wav file")
    print(COMPOSE_WAV + " : for compose wav file")
    print(EXIT + " : for exiting the program")
    act = input()
    if act != MODIFY_WAV and act != COMPOSE_WAV and act != EXIT:
        return "error"
    else:
        return act


def gen_menu():
    """this function uses the gen_input function,it prints an error massage if
     the input of the user was wrong and asks him to choose again,
     in the end returns the users choice"""
    gen_choice = gen_input()  # use of gen_input function to get the users
    # choice
    while gen_choice == "error":  # while the input is wrong
        print("Syntax is not in the right format")
        gen_choice = gen_input()  # choose again
    return gen_choice  # the final choice


def print_menu_modify():
    """this function prints to the screen the modifying menu and asks for the
    users choice,if the input is wrong returns an error else returns the
     users choice"""
    print("Choose action:")
    print(REVERSE + " : for reversing the audio data")
    print(NEGATIVE_AUDIO + " : for negative the audio data")
    print(ACCELERATION + " : for accelerating the audio data")
    print(SLOW_DOWN + " : for slowing down the audio data")
    print(INCREASING_VOLUME + " : for increasing volume")
    print(DECREASING_VOLUME + ": for decreasing volume")
    print(LOW_PASS_FILTER + ": for low pass filter")
    print(EXIT_MODIFY + ": back to gen_menu")
    act = input()
    if act != REVERSE and act != NEGATIVE_AUDIO and act != ACCELERATION and \
            act != SLOW_DOWN and act != INCREASING_VOLUME and \
            act != DECREASING_VOLUME and \
            act != LOW_PASS_FILTER and act != EXIT_MODIFY:
        return "error"
    else:
        return act


def modify_wav():
    """this function asks from the user the name of the file he wants to modify
    ,checks if there are no problems with the file and updates the sample rate
     and the audio data depend on the users choice in mod_ wav_ menu function
      until he chose to exit the modifying"""
    filename = input("what is the name of the file?")
    file_data = wave_helper.load_wave(filename)  # save what load_wave returned
    while file_data == -1:  # problem with the file
        filename = input("the name not in the right format ,what is the name "
                         "of the file?")  # asking for input again
        file_data = wave_helper.load_wave(filename)
    sample_rate, audio_data = file_data  # tuple
    go_to_gen_menu = True
    while go_to_gen_menu:  # while the user wants to continue the modifying
        go_to_gen_menu, audio_data = mod_wav_menu(audio_data)
        # mod_wav_menu returns boolean value and nested lists(audio_data)
    return sample_rate, audio_data  # after user asked to exit


def mod_wav_menu(audio_data):
    """this function gets the audio data list, uses some functions
    to do the action that the user chose in the print_menu_modify function.
    the function returns the updated audio data and a boolean value if the user
     wants to continue modifying or not"""
    action = print_menu_modify()  # users choice
    while action == "error":  # invalid choice
        print("invalid input")
        action = print_menu_modify()
    if action == REVERSE:  # chose 1
        print("the process was done")
        return True, audio_data[::-1]  # reversing the audio_data
    if action == NEGATIVE_AUDIO:  # chose 2
        print("the  process  was done")
        return True, negative_audio(audio_data)  # negative the audio data
    if action == ACCELERATION:  # chose 3
        print("the  process  was done")
        return True, audio_acceleration(
            audio_data)  # accelerate the audio data
    if action == SLOW_DOWN:  # chose 4
        print("the  process  was done")
        return True, audio_slow_down(audio_data)  # slow down the audio data
    if action == INCREASING_VOLUME:  # chose 5
        print("the process  was done")
        return True, change_speed_volume(audio_data,
                                         "+")  # increasing the volume
    if action == DECREASING_VOLUME:  # chose 6
        print("the  process  was done")
        return True, change_speed_volume(audio_data,
                                         "-")  # decreasing the volume
    if action == LOW_PASS_FILTER:  # chose 7
        print("the  process  was done")
        return True, low_pass_filter(audio_data)  # low pass the audio data
    if action == EXIT_MODIFY:  # chose 8
        return False, audio_data  # the final audio data


def negative_audio(audio_data):
    """this function gets a nested lists of audio data.it converts every value
     to its negative version(multiply the value by -1),
     in the end returns the final audio data"""
    new_audio_lst = audio_data
    for i in range(len(audio_data)):
        for j in range(len(audio_data[0])):
            reverse = new_audio_lst[i][j] * -1
            if reverse > 32767:  # cant be more than the maximal value
                new_audio_lst[i][j] = 32767
            else:
                new_audio_lst[i][j] = new_audio_lst[i][j] * -1
    return new_audio_lst


def audio_acceleration(audio_data):
    """this function gets the audio data,and updates it to twice faster"""
    return [audio_data[i] for i in range(len(audio_data)) if i % 2 == 0]


def average_slow_down(lst_1, lst_2):
    """this function gets 2 lists and returns the average of them"""
    return [int((lst_1[0] + lst_2[0]) / 2), int((lst_1[1] + lst_2[1]) / 2)]


def audio_slow_down(audio_data):
    """this function gets the audio data and updates the audio twice slower"""
    new_audio_lst = copy.deepcopy(audio_data)  # the same list like the audio
    # data list
    for i in range(1, len(audio_data)):
        new_audio_lst.insert((2 * i) - 1, average_slow_down(audio_data[i - 1],
                                                            audio_data[i]))
    return new_audio_lst  # the updated audio data


def change_speed_volume(audio_data, operator):
    """this function gets the audio data and an operator,if the operator is "+"
    it turns the volume up by multiplying every value in the list by 1.2
    if the operator is "-" it turns the volume down by dividing every value
    by 1.2.the function return the final audio data"""
    new_lst = audio_data
    for i in range(len(audio_data)):
        for j in range(len(audio_data[0])):
            if operator == "+":  # volume up
                multiplication = int(new_lst[i][j] * 1.2)
                if multiplication > 32767:  # cant be bigger than the max value
                    new_lst[i][j] = 32767
                elif multiplication < -32768:  # cant be smaller than the
                    # min value
                    new_lst[i][j] = -32768
                else:
                    new_lst[i][j] = multiplication
            else:  # operator "-" volume down
                multiplication = int(new_lst[i][j] / 1.2)
                new_lst[i][j] = multiplication
    return new_lst  # updated audio data


def average_low_pass_filter(lst_1, lst_2, lst_3):
    """this function gets 3 lists and returns their average"""
    return [int((lst_1[0] + lst_2[0] + lst_3[0]) / 3),
            int((lst_1[1] + lst_2[1] + lst_3[1]) / 3)]


def low_pass_filter(audio_data):
    """this function gets an audio data and blurs it"""
    new_audio_lst = copy.deepcopy(audio_data)
    new_audio_lst[0] = average_slow_down(audio_data[0], audio_data[1])
    for i in range(1, len(new_audio_lst) - 1):
        new_audio_lst[i] = average_low_pass_filter(audio_data[i - 1],
                                                   audio_data[i],
                                                   audio_data[i + 1])
    new_audio_lst[len(audio_data) - 1] = average_slow_down(audio_data[-1],
                                                           audio_data[-2])
    return new_audio_lst  # the updated audio data


def get_compose_list():
    """this function takes the file that the user chose(file with instructions
    for the compose) and converts it to list of letters and numbers(in the
    format of letter and number next to each other),
    the function returns the final compose list"""
    compose_list = []
    filename = input(
        "what is the name of the file?")  # file for composing
    while os.path.isfile(filename) is False:  # the file not exist
        filename = input("Wrong input! What is the name of the file?")
    file_comp = open(filename)
    for line in file_comp:
        line = line.strip()
        last_char = False
        for char in line:
            if last_char:
                if char.isdigit():
                    # רוצים להוסיף לאיבר האחרון ברשימה את המספר הבא
                    # אני לוקח את המיקום של המספר וכדי להכניס את המספר החדש אני יוצר מספר חדש שמורכב מהמספר הישן והחדש
                    compose_list[len(compose_list) - 1][1] = \
                        compose_list[len(compose_list) - 1][1] * 10 + int(char)
                    continue
            elif char.isdigit():  # האיבר הזה מספר אבל הקודם לא
                compose_list[len(compose_list) - 1].append(int(char))
            elif (65 <= ord(char) and ord(char) <= 71) or char == "Q":
                # זה תו מתאים ולכן נכניס אותו בתור רשימה חדשה לרשימה
                compose_list.append([])
                compose_list[len(compose_list) - 1].append(char)
                last_char = False
    return compose_list


def get_freq(char):
    """this function gets a note and returns it frequency"""
    if char == "A":
        return 440
    if char == "B":
        return 494
    if char == "C":
        return 523
    if char == "D":
        return 587
    if char == "E":
        return 659
    if char == "F":
        return 698
    if char == "G":
        return 784
    return 1


def get_sample(sample_num, samples_per_seconds):
    """this function calculates...."""
    result = sample_num / samples_per_seconds
    result = math.sin(2 * math.pi * result)
    return int(result * MAX_VOLUME)


def get_compose():
    """this function returns the final audio list depend on the returned
    compose list from the get_compose_function and the sample rate(2000)"""
    orders_lst = get_compose_list()
    compose_lst = []
    for i in range(len(orders_lst)):
        frequency = get_freq(orders_lst[i][0])
        for j in range(int(orders_lst[i][1] * 2000 / 16)):  # למה הטווח עד פה?
            if orders_lst[i][0] == "Q":  # quiet
                compose_lst.append([0, 0])
            else:  # not "q"
                samples_per_seconds = 2000 / frequency
                sample = get_sample(j, samples_per_seconds)  # ?
                compose_lst.append([sample, sample])
    return 2000, compose_lst


def main():
    """this function does some action with audio wav depend on the users
     choice(modifying audio wav,compose audio), when the user choose to exit in
      the gen_menu the process stops after saving all the changes with the
      sava_wave function"""
    action = gen_menu()  # users choice
    while action != EXIT:  # while user didnt choose 3
        if action == MODIFY_WAV:  # chose 1
            sample_rate, audio_data = modify_wav()  # do actions on the
            # audio data
            # when the user chooses 8 in the modify menu
            filename = input("Enter the name of the file to put the data on?")
            completed_insert = wave_helper.save_wave(sample_rate,
                                                     audio_data, filename)  #
            # save the changes
            while completed_insert == -1:  # problem with the parameters in
                # the save wave function
                print("there was a problem with the name.")
                filename = input(
                    "Please enter the name of the file to put the data on?")
                completed_insert = wave_helper.save_wave(sample_rate,
                                                         audio_data, filename)
            action = gen_menu()
            continue
        elif action == COMPOSE_WAV:  # chose 2
            sample_rate, audio_data = get_compose()
            go_to_gen_menu = True
            while go_to_gen_menu:
                go_to_gen_menu, audio_data = mod_wav_menu(audio_data)  #
                # goes to the modifying menu,users can choose what action to do
                # with the audio data that was made
            filename = input("Enter the name of the file to put the data on?")
            completed_insert = wave_helper.save_wave(sample_rate,
                                                     audio_data, filename)
            while completed_insert == -1:  # problem with the parameters in
                # the save wave function
                print("there was a problem with the name.")
                filename = input(
                    "Please enter the name of the file to put the data on?")
                completed_insert = wave_helper.save_wave(sample_rate,
                                                         audio_data, filename)
            action = gen_menu()


if __name__ == '__main__':
    main()
