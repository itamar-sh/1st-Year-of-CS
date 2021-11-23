import tkinter as tk
from typing import Callable, List, Any

MINUTES = 3
SECOND = 59
INIT_SECONDS = 1
SCORE = "0"
BUTTON_HOVER_COLOR = 'gray'
REGULAR_COLOR = 'ivory2'
BUTTON_ACTIVE_COLOR = 'slateblue'
BUTTON_STYLE = {"font": ("Courier", 30),
                "borderwidth": 1,
                "relief": tk.RAISED,
                "bg": REGULAR_COLOR,
                "activebackground": BUTTON_ACTIVE_COLOR}
INFO_BUTTON_STYLE = {"font": ("Courier", 10),
                "borderwidth": 1,
                "relief": tk.RAISED,
                "bg": REGULAR_COLOR,
                "activebackground": BUTTON_ACTIVE_COLOR}
GAME_RULES = "BOGGLE: Game Rules \n \
1) You have 3 minutes                                               \n \
2) You need to find as much words as you can                        \n \
3) All the buttons for one word have to be connected                \n \
4) Repeating the same button twice in one word is forbidden         \n \
5) Once you press \"Enter\" the clock is running!                     \n \
 Good Luck!"

GAME_FINISH = " you played very well \n \
click Yes if you want to play again and No if not"


class BoggleGUI:

    def __init__(self, board, list_score) -> None:
        """
        initialize all the widgets in the game
        :param board: list of list of strings
        :param list_score: list of all the scores of all the games
        """
        self._window = tk.Tk()
        self.list_score = list_score
        self.score = SCORE
        self._board = board
        self._buttons = {}
        self._last_button_pressed = []
        self.init_frames()
        self.arrange_frames_by_grids()
        self.init_labels()
        self.init_buttons()
        self.arrange_buttons_and_labels_by_grids()
        self.minutes = MINUTES
        self.second = INIT_SECONDS

    def run(self) -> None:
        """
        run the game in loops
        :return: None
        """
        self._window.mainloop()

    def init_frames(self):
        """
        initialize all the frames in the game
        :return: None
        """
        self._outer_frame = tk.Frame(self._window, bg='old lace',
                                     highlightbackground='dark slate grey',
                                     highlightthickness=5)
        self._outer_frame.pack(side=tk.TOP, fill=tk.BOTH, expand=True)
        self._buttons_frame = tk.Frame(self._outer_frame, highlightbackground='dark slate grey')
        self._info_frame = tk.Frame(self._outer_frame, highlightbackground='dark slate grey')
        self._command_frame = tk.Frame(self._window)

    def arrange_frames_by_grids(self):
        """
        take every frame and initialize grid on him
        :return:
        """
        for i in range(4):
            tk.Grid.columnconfigure(self._info_frame, i, weight=1)
        for i in range(8):
            tk.Grid.rowconfigure(self._info_frame, i, weight=1)
        for i in range(12):
            tk.Grid.columnconfigure(self._command_frame, i, weight=1)
        for i in range(4):
            tk.Grid.columnconfigure(self._buttons_frame, i, weight=1)
        for i in range(4):
            tk.Grid.rowconfigure(self._buttons_frame, i, weight=1)

    def init_buttons(self):
        """
        initialize all the buttons in the game
        :return: None
        """
        # welcome screen
        self._entry_button = tk.Button(self._outer_frame, text="Enter",
                                       command=self.first_enter, bg="ivory2")
        self._entry_button.pack(side=tk.TOP)
        # left side game screen
        self._enter_button = tk.Button(self._command_frame, text="Enter", **INFO_BUTTON_STYLE)
        self._delete_button = tk.Button(self._command_frame, text="Delete", **INFO_BUTTON_STYLE)
        self._buttons["Enter"] = self._enter_button
        self._buttons["Delete"] = self._delete_button
        # right side game screen
        for row in range(len(self._board)):
            for col in range(len(self._board[0])):
                self.make_button(row, col)  # init + grid + colors + enter dict
        # finish screen
        self._yes_button = tk.Button(self._outer_frame, text="YES")
        self._no_button = tk.Button(self._outer_frame, text="No")
        self._buttons["Yes"] = self._yes_button
        self._buttons["No"] = self._no_button

    def init_labels(self):
        """
        initialize all the labels in the game
        :return: None
        """
        # welcome screen
        self._display_label = tk.Label(self._outer_frame,
                                        font=("Courier", 30),
                                        bg='ivory3', width=23,
                                        relief="ridge")
        self._display_label["text"] = "Welcome"
        self._display_label.pack(side=tk.TOP, fill=tk.BOTH, expand=False)
        self._title_label = tk.Label(self._outer_frame,
                                        font=("Courier", 10),
                                        bg='antique white', width=20,
                                        relief="ridge")
        self._title_label["text"] = GAME_RULES
        self._title_label.pack(side=tk.TOP, fill=tk.BOTH)
        # left game screen
        self.clock_lbl = tk.Label(self._command_frame, font=('calibri', 40, 'bold'),
                         background='antique white')
        self._points_label = tk.Label(self._info_frame,
                                      font=("Courier", 20),
                                      bg='antique white', width=10,
                                      relief="ridge")
        self._points_label["text"] = "Score: 0"
        self._completed_words_label = tk.Label(self._info_frame,
                                               font=("Courier", 10),
                                               bg='antique white', width=10,
                                               relief="ridge")
        self._completed_words_title_label = tk.Label(self._info_frame,
                                               font=("Courier", 12),
                                               bg='antique white', width=10,
                                               relief="ridge")
        self._completed_words_title_label["text"] = "Used Words "
        self._completed_words_label["text"] = ""
        # finish screen
        self._finish_label = tk.Label(self._outer_frame,
                                        font=("Courier", 15),
                                        bg='ivory2', width=23,
                                        relief="ridge")
        self._finish_label["text"] = GAME_FINISH
        self._score_label = tk.Label(self._outer_frame,
                                        font=("Courier", 15),
                                        bg='ivory2', width=23,
                                        relief="ridge")

    def arrange_buttons_and_labels_by_grids(self):
        """
        place not words buttons and labels to the right place in the frames
        :return: None
        """
        self._points_label.grid(row=0, column=0, rowspan=1,
                    columnspan=4, sticky=tk.NSEW)
        self._completed_words_title_label.grid(row=1, column=0, rowspan=1,
                                         columnspan=4, sticky=tk.NSEW)
        self._completed_words_label.grid(row=2, column=0, rowspan=6,
                    columnspan=4, sticky=tk.NSEW)
        self._enter_button.grid(row=0, column=5, rowspan=1,
                    columnspan=4, sticky=tk.NSEW)
        self._delete_button.grid(row=0, column=9, rowspan=1,
                    columnspan=3,ipady = 20, sticky=tk.NSEW)
        self.clock_lbl.grid(row=0, column=0, rowspan=4,
                    columnspan=5, sticky=tk.NSEW)

    def make_button(self, row, col):
        """
        initiliaze the words buttons
        :param row: coordinate of word button in the board
        :param col: coordinate of word button in the board
        :return: None
        """
        button = tk.Button(self._buttons_frame, text = str(self._board[row][col]), **BUTTON_STYLE)
        button.grid(row=row, column=col, rowspan=1,
                    columnspan=1, sticky=tk.NSEW)
        self._buttons[str((row, col))] = button  # each button name is : "(x, y)"
        def _on_enter(event: Any) -> None:
            if button['background'] == REGULAR_COLOR:
                button['background'] = BUTTON_HOVER_COLOR

        def _on_leave(event: Any) -> None:
            if button['background'] == BUTTON_HOVER_COLOR:
                button['background'] = REGULAR_COLOR

        button.bind("<Enter>", _on_enter)
        button.bind("<Leave>", _on_leave)
        return button

    # after the user start enter for beginning the game
    def first_enter(self) -> None:
        """
        end the welcome screen and call the function to make the game screen
        :return: None
        """
        self._title_label.destroy()
        self._entry_button.destroy()
        self.make_game_screen()

    # init the game screen
    def make_game_screen(self):
        """
        pack the frames connect to the game screen and start the watch
        :return: None
        """
        self._buttons_frame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
        self._info_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        self._command_frame.pack(side=tk.BOTTOM, fill=tk.BOTH, expand=True)
        self.time()

    # run after time finished
    def finish_game(self):
        """
        end the frames connect to the game screen and call to the function
        that initialize the final screen
        :return: none
        """
        self._info_frame.destroy()
        self._buttons_frame.destroy()
        self._display_label.destroy()
        self._command_frame.destroy()
        self.make_finish_screen()

    # after method finish_game run
    def make_finish_screen(self):
        """
        pack the finish game screen
        :return: None
        """
        self._display_label = tk.Label(self._outer_frame,
                                        font=("Courier", 35),
                                        bg='ivory3', width=23,
                                        relief="ridge")
        self._display_label["text"] = "Game Over!"
        self.list_score.append(self.score)
        self._score_label["text"] = "your game score: " + str(
            self.score) + "   the high score: " + max(self.list_score)
        self._display_label.pack(side=tk.TOP, fill=tk.BOTH)
        self._finish_label.pack(side=tk.TOP, fill=tk.BOTH)
        self._score_label.pack(side=tk.TOP, fill=tk.BOTH)
        self._yes_button.pack(side=tk.TOP)
        self._no_button.pack(side=tk.TOP)

    def time(self):
        """
        start the watch, when it ends call "finish game"/
        :return: None
        """
        stop = False
        if self.second == 0:
            self.minutes -= 1
            self.second = SECOND
            if self.minutes == -1:
                stop = True
        else:
            self.second -= 1
        if len(str(self.second)) == 1:
            string = ":0"
        else:
            string = ":"
        clock = str(self.minutes) + string + str(self.second)
        self.clock_lbl.config(text=clock)
        if stop is False:
            self.clock_lbl.after(1000, self.time)
        else:
            self.finish_game()

    def set_button_command(self, button_name: str, cmd: Callable[[], None]) -> None:
        """
        give every button in the dectionary of buttons a function.
        :param button_name: str
        :param cmd: func
        :return: None
        """
        self._buttons[button_name].configure(command=cmd)

    def get_button_chars(self) -> List[str]:
        """
        gets the names of all the buttons in the dictionary of buttons
        :return: list of strings
        """
        return list(self._buttons.keys())

    def set_label(self, info_tuple):
        """
        change the labels during the actual game
        take of the list of used words, the score and the display screen
        :param info_tuple: tuple with str. list of str and str
        :return: None
        """
        display, list_of_finished_words, score = info_tuple
        self._display_label["text"] = display
        display_text = ""
        for word in list_of_finished_words:
            display_text = display_text + word + "\n"
        self._completed_words_label["text"] = display_text
        self.score = str(score)
        self._points_label["text"] = "Score: " + self.score

    def change_background(self, name_of_button):
        """
        change the background of the clicked word button
        :param name_of_button: str
        :return: None
        """
        if name_of_button[0] == "(":
            self._buttons[name_of_button]["bg"] = "tan1"

    def reset_backgrounds(self, name_of_button):
        """
        after pressed "Enter" we reset the backgrounds of all
        the clicked buttons
        :param name_of_button: str
        :return: None
        """
        self._last_button_pressed = []
        for button in self._buttons.values():
            if button["bg"] == "tan1":
                button['background'] = REGULAR_COLOR

    def change_last_pressed(self, name_of_button):
        """
        we remember the clicked buttons to remove he background one by one.
        :param name_of_button: str
        :return: None
        """
        if name_of_button[0] == "(":
            self._last_button_pressed.append(name_of_button)

    def reset_last_background(self):
        """
        after pressed "delete" we remove the last clicked button
        :return: None
        """
        if self._last_button_pressed:
            self._buttons[self._last_button_pressed[-1]]["bg"] = REGULAR_COLOR
            self._last_button_pressed.pop()

    def get_score(self):
        """
        :return: str represents the current score in the game
        """
        return self.score

    def quit(self):
        """
        close the main tkinter object
        :return: None
        """
        self._window.destroy()





