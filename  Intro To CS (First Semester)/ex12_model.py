from ex12_utils import *
from boggle_board_randomizer import randomize_board
from typing import List, Tuple

MAX_LENGTH = 16
MIN_LENGTH = 3


class BoggleModel:

    def __init__(self, file_path) -> None:
        """
        initializes the model of the game, do all the logic part of the game
        :param file_path: str of file path to read
        """
        self._letter_pressed = []
        self._board = randomize_board()
        self._word_dict = load_words_dict(file_path)
        self._score = 0
        self._list_of_word = []
        self._path = []
        self._path_word_dict = {}
        for n in range(MIN_LENGTH, MAX_LENGTH + 1):
            option_of_words = find_length_n_words(n, self._board, self._word_dict)
            self._path_word_dict[n] = option_of_words

    def type_in(self, c: str) -> None:
        """
        check which button is pressed and calls to function accordingly
        :param c: text of button pressed
        """
        if c == "Enter":
            word = self.check_word()
            if word and word not in self._list_of_word:
                self._list_of_word.append(word)
                self._score += (len(word) ** 2)
            self._letter_pressed = []
            self._path = []
        elif c == "Delete":
            if self._letter_pressed:
                self._letter_pressed.pop()
                self._path.pop()
        else:
            self.append_letter(c)

    def check_word(self) -> str:
        """
        check if is valid path on the board and the word exist in the dict
        :return: word if valid and none if not
        """
        n = len(self._path)
        if MIN_LENGTH <= n <= MAX_LENGTH:
            for pair in self._path_word_dict[n]:
                if pair[0] == "".join(self._letter_pressed) and pair[1] == self._path:
                    return "".join(self._letter_pressed)

    def get_board(self) -> List[List[str]]:
        return self._board

    def get_display(self) -> Tuple[str, List[str], int]:
        return "".join(self._letter_pressed), self._list_of_word, self._score

    def append_letter(self, c: str) -> None:
        """
        append letter to the path and to the letter pressed
        :param c: index of letter pressed
        """
        x = int(c[1])
        y = int(c[4])
        letter = self._board[x][y]
        if (x, y) not in self._path:
            self._letter_pressed += [letter]
            self._path.append((x, y))
