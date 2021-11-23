#################################################################
# FILE : boggle.py
# WRITERS : Itamar Shechter , itamar.sh , 315092759
#           Merav diamant , diamant.merav, 208009456
# EXERCISE : intro2cs2 ex12 2020
# DESCRIPTION: boggle game
# STUDENTS I DISCUSSED THE EXERCISE WITH: No one
# WEB PAGES I USED: i didn't use any web page
# we use the calculator program that you sent to us
##################################################################

from typing import Callable, Tuple, Any
from ex12_model import BoggleModel
from ex12_GUI import BoggleGUI

INIT_SCORE = 0


class BoggleController:
    """controller of the game initializes the model and gui of the game make all the connect between them"""
    def __init__(self, file_path: str, list_score) -> None:
        self._another_game = False
        self._score = INIT_SCORE
        self._model = BoggleModel(file_path)
        self._gui = BoggleGUI(self._model.get_board(), list_score)
        for button_text in self._gui.get_button_chars():  # connect action for each button
            action = self.create_button_action(button_text)
            self._gui.set_button_command(button_text, action)

    def create_button_action(self, button_text: str) -> Callable[[], None]:
        """
        Suitable for each button the appropriate action by an internal function
        :param button_text: str
        :return: func
        """
        def fun() -> None:
            if button_text == "Yes":
                self._another_game = True
                self._score = self._gui.get_score()
                self._gui.quit()
                return
            if button_text == "No":
                self._gui.quit()
                return
            self._model.type_in(button_text)
            self._gui.set_label(self._model.get_display())
            if button_text == "Enter":
                self._gui.reset_backgrounds(button_text)
            elif button_text == "Delete":
                self._gui.reset_last_background()
            else:
                self._gui.change_background(button_text)
            self._gui.change_last_pressed(button_text)

        return fun

    def run(self) -> Tuple[bool, Any]:
        """
        run the game and checks if the player wants another one
        :return: Tuple, the player answer and game score
        """
        self._gui.run()
        if self._another_game:
            return True, self._score
        else:
            return False, None


def main():
    """
    the main function initializes a new controller while the player want to play
    """
    more_game = True
    list_score = []
    while more_game is True:
        game = BoggleController("boggle_dict.txt", list_score)
        more_game, score = game.run()
        list_score.append(score)


if __name__ == "__main__":
    main()
