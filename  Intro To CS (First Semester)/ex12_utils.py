from typing import List, Tuple, Dict, Any


MEX_VALUE = 3
MIN_VALUE = 0


def load_words_dict(file_path: str) -> Dict[str, bool]:
    """
    create dict of all the word in the file text in the key and true in the value
    :param file_path: str of file path to read
    :return: Dict of word
    """
    words_dict = {}
    with open(file_path, "r") as file:
        for line in file:
            words_dict[line[:-1]] = True
    return words_dict


def is_valid_path(board: List[List[str]], path: List[Tuple[int, int]], words: Dict[str, bool]) -> Any:
    """
    check if the path is legal and the word exists in the dict
    :param board: list of list of all the letter in the bored
    :param path: list of the index of the chosen letter in the bored
    :param words: dict of the optional word that the user can guess
    :return: str - word if this is a valid word or none if not
    """
    if len(set(path)) != len(path) or check_path(path) is False:
        return
    my_word = ""
    for pair in path:
        if pair[0] > MEX_VALUE or pair[0] < MIN_VALUE or pair[1] > MEX_VALUE or pair[1] < MIN_VALUE:
            return
        my_word += board[pair[0]][pair[1]]
    if my_word in words.keys() and words[my_word]:
        return my_word


def find_length_n_words(n: int, board: List[List[str]], words: Dict[str, bool])\
        -> List[Tuple[str, List[Tuple[int, int]]]]:
    """
    search for all word in this length that valid in the bored, use helper recursive func make_path_and_word
    :param n: int - len path of word
    :param board: list of list of all the letter in the bored
    :param words: dict of the optional word that the user can guess
    :return: list of all the valid word and there path
    """
    word_list = [i for i in words.keys()]
    word_list.sort()
    result = []
    for i in range(4):
        for j in range(4):
            paths = []
            path = []
            word = board[i][j]
            make_path_and_word(n, (i, j), path, paths, word, board, word_list)
            for res in paths:
                new_word = is_valid_path(board, res, words)
                if new_word is not None:
                    result.append((new_word, res))
    return result


def make_path_and_word(n: int, location: Tuple[int, int], path: List[Tuple[int, int]],
                       paths: List[List[Tuple[int, int]]], word: str, board: List[List[str]], words_list: List[str]):
    """
    recursive func that add to paths all the optional paths by the possible direction in the board
    :param words_list: list of all the word in the dict
    :param board: list of list of all the letter in the bored
    :param word: start of word until now
    :param n: int - len path of word
    :param location: tuple of index of letter
    :param path: list of the index of the chosen letter in the board
    :param paths: list of all optional paths in the board
    """
    path.append(location)
    if len(path) == n:
        paths.append(path[:])
    elif oppenning_search(words_list, 0, len(words_list) - 1, word) == -1:
        return
    else:
        for new_loc in call_new_locations(location):
            if new_loc not in path:
                letter = board[new_loc[0]][new_loc[1]]
                make_path_and_word(n, new_loc, path, paths, word + letter, board, words_list)
                path.pop()


def check_path(path: List[Tuple[int, int]]) -> bool:
    """
    pass on all 2 pair in the path and checks if the distance between the index on each side
    is small or equal to 1 if all of them is right return true else false
    :param path: list of the index of the chosen letter in the board
    :return: bool
    """
    for index in range(len(path) - 1):
        x = path[index][0] - path[index + 1][0]
        y = path[index][1] - path[index + 1][1]
        if abs(x) > 1 or abs(y) > 1:
            return False
    return True


def call_new_locations(location: Tuple[int, int]) -> List[Tuple[int, int]]:
    """
    get location and append all the direction that possible
    :param location: tuple of index of letter
    :return: list of all the possible move
    """
    results = []
    if location[0] > 0:
        results.append((location[0] - 1, location[1]))
    if location[0] < 3:
        results.append((location[0] + 1, location[1]))
    if location[1] > 0:
        results.append((location[0], location[1] - 1))
    if location[1] < 3:
        results.append((location[0], location[1] + 1))
    if location[0] > 0 and location[1] > 0:
        results.append((location[0] - 1, location[1] - 1))
    if location[0] > 0 and location[1] < 3:
        results.append((location[0] - 1, location[1] + 1))
    if location[1] > 0 and location[0] < 3:
        results.append((location[0] + 1, location[1] - 1))
    if location[0] < 3 and location[1] < 3:
        results.append((location[0] + 1, location[1] + 1))
    return results


def oppenning_search(list_of_words, left, right, openning):
    """
    We check if the current word is in our list of words
    if not than return -1 if yes return the index
    :param list_of_words: List of str than can be answers
    :param left: index that close the search from the left
    :param right: index that close the search from the right
    :param openning: str that represents the current word to check
    :return: int, -1
    """
    while left <= right:
        mid = left + (right - left) // 2
        if list_of_words[mid].startswith(openning):
            return mid
        elif list_of_words[mid] < openning:
            left = mid + 1
        else:
            right = mid - 1
    return -1
