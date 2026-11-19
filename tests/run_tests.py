#!/usr/bin/env python3
"""Небольшие автоматические проверки всех заданий ЛР2."""
import random
import re
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BIN = ROOT / 'lr2'
CHECKS = 0


def run(text, *args, timeout=7):
    p = subprocess.run([str(BIN), *args], input=text, text=True,
                       capture_output=True, timeout=timeout)
    assert p.returncode == 0, f'Ошибка программы: {p.stderr}'
    assert 'ОШИБКА поиска' not in p.stdout, p.stdout
    assert 'Ошибка хеширования' not in p.stdout, p.stdout
    return p.stdout


def check(condition, message):
    global CHECKS
    assert condition, message
    CHECKS += 1


def correct_parentheses(text):
    stack = []
    removed = set()
    for i, char in enumerate(text):
        if char == '(':
            stack.append(i)
        elif char == ')':
            if stack:
                stack.pop()
            else:
                removed.add(i)
    removed.update(stack)
    return ''.join(c for i, c in enumerate(text) if i not in removed)


rng = random.Random(2026)
for _ in range(60):
    s = ''.join(rng.choice('ab()!') for _ in range(rng.randint(0, 90)))
    out = run(f'1\n{s}\n0\n')
    found = re.search(r'Результат: ([^\n]*)', out)
    check(bool(found) and found.group(1) == correct_parentheses(s),
          f'Скобки: {s!r}')

out = run('2\nSETADD 10\nSETADD 20\nSETADD 10\nSET_AT 10\nSETDEL 10\nSET_AT 10\nPRINT\nEXIT\n0\n')
check('SET> TRUE\nSET> TRUE\nSET> FALSE\nSET> TRUE\nSET> TRUE\nSET> FALSE\nSET> {20}' in out, 'Операции множества')

with tempfile.TemporaryDirectory() as temp:
    path = str(Path(temp) / 'set.txt')
    for command, expected in [('SETADD -1', 'TRUE'), ('SETADD -1', 'FALSE'),
                              ('SET_AT -1', 'TRUE'), ('SETDEL -1', 'TRUE'),
                              ('SET_AT -1', 'FALSE')]:
        output = run('', '--file', path, '--query', command)
        check(output.strip() == expected, f'Файловое множество: {command}')

# Проверка расширения хеш-таблицы множества и длинных цепочек.
many = [101 * value for value in range(550)] + [-101 * value - 1 for value in range(1, 101)]
commands = [f'SETADD {value}' for value in many]
commands += ['SET_AT 0', 'SET_AT 5555', 'SET_AT -102',
             'SETDEL 0', 'SET_AT 0', 'PRINT', 'EXIT']
out = run('2\n' + '\n'.join(commands) + '\n0\n', timeout=15)
check('SET> TRUE\nSET> TRUE\nSET> TRUE\nSET> TRUE\nSET> FALSE' in out,
      'Множество: поиск/удаление после увеличения таблицы')
printed = re.search(r'SET> \{([^}]+)\}', out)
check(bool(printed), 'Множество: PRINT после расширения')
printed_values = {int(part.strip()) for part in printed.group(1).split(',')}
check(printed_values == set(many) - {0}, 'Множество: все ключи сохранились после grow()')

out = run('3\n4\n1 2 3\n2 3 4\n5 6\n3 4 5\n0\n')
check('Общих элементов: 2' in out and 'Множество 1: {1, 2, 3}' in out, 'Пересечение')

for _ in range(20):
    n = rng.randint(1, 50)
    blocks = [(rng.randint(1, 20), rng.randint(1, 100)) for _ in range(n)]
    expected = sum(max(h for w, h in blocks if w == width)
                   for width in {w for w, _ in blocks})
    data = '4\n' + str(n) + '\n' + ''.join(f'{w} {h}\n' for w, h in blocks) + '0\n'
    check(f'Максимальная высота: {expected}' in run(data), 'Пирамида')


class Node:
    def __init__(self, value):
        self.value, self.left, self.right = value, None, None


def right_view(values):
    root = None
    for value in values:
        if root is None:
            root = Node(value)
            continue
        node = root
        while True:
            if node.value == value:
                break
            side = 'left' if value < node.value else 'right'
            child = getattr(node, side)
            if child is None:
                setattr(node, side, Node(value))
                break
            node = child
    if root is None:
        return []
    q, result = [root], []
    while q:
        result.append(q[-1].value)
        q = [child for node in q for child in (node.left, node.right) if child]
    return result


for _ in range(25):
    values = [rng.randint(-200, 200) for _ in range(rng.randint(1, 70))]
    values = [v for v in values if v != 0]
    expected = 'Правый край:' + ''.join(f' {v}' for v in right_view(values))
    out = run('5\n' + ' '.join(map(str, values)) + ' 0\n0\n')
    check(expected in out, 'BST')

for i in range(20):
    count = rng.randint(1, 80)
    keys = [rng.randint(-20000, 20000) for _ in range(count)]
    if i % 2 == 0:
        keys = [v * 1009 for v in keys]  # набор с коллизиями свёртки
    data = f'6\n{rng.randint(3, 17)}\n' + ' '.join(map(str, keys)) + '\n0\n'
    out = run(data, timeout=15)
    check('Цепочки: сравнений' in out and 'Кукушка: вытеснений' in out,
          'Хеш-таблицы')


def folded(value):
    value = abs(value)
    result = 0
    while True:
        result += value % 1000
        value //= 1000
        if value == 0:
            return result


for value in [0, 1, -123456789, 523456795, 2**63-1, -(2**63)]:
    out = run(f'7\n{value}\n0\n')
    check(f'Индекс корзины: {folded(value) % 1009}' in out, 'Свёртка')
    check('Поиск в кукушке: TRUE' in out, 'Поиск после свёртки')


def reference_lfu(capacity, commands):
    entries = {}
    clock = 0
    output = []
    for command in commands:
        name, key, *values = command.split()
        key = int(key)
        if name == 'GET':
            if key not in entries:
                output.append(-1)
            else:
                val, freq, _ = entries[key]
                clock += 1
                entries[key] = (val, freq + 1, clock)
                output.append(val)
            continue
        value = int(values[0])
        if capacity <= 0:
            continue
        clock += 1
        if key in entries:
            _, freq, _ = entries[key]
            entries[key] = (value, freq + 1, clock)
        else:
            if len(entries) == capacity:
                victim = min(entries, key=lambda k: (entries[k][1], entries[k][2]))
                del entries[victim]
            entries[key] = (value, 1, clock)
    return output


for _ in range(40):
    capacity = rng.randint(0, 8)
    commands = []
    for _ in range(rng.randint(1, 55)):
        key = rng.randint(-6, 6)
        commands.append(f'GET {key}' if rng.randint(0, 1) else f'SET {key} {rng.randint(-300, 300)}')
    raw = f'8\n{capacity}\n{len(commands)}\n' + '\n'.join(commands) + '\n0\n'
    out = run(raw)
    part = out.split('Команды: SET ключ значение / GET ключ\n', 1)[1].split('\n1. Стек', 1)[0]
    actual = [int(line) for line in part.splitlines() if re.fullmatch(r'-?\d+', line)]
    check(actual == reference_lfu(capacity, commands), f'LFU: {commands}')

print(f'OK: {CHECKS} автоматических проверок пройдено.')
