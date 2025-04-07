def rail_fence_encrypt(text, num_rails):
    if num_rails == 1:
        return text
    rails = [''] * num_rails
    rail = 0
    direction = 1
    for char in text:
        rails[rail] += char
        rail += direction
        if rail == 0 or rail == num_rails - 1:
            direction *= -1
    return ''.join(rails)


def rail_fence_decrypt(cipher, num_rails):
    if num_rails == 1:
        return cipher
    n = len(cipher)
    pattern = [['\n'] * n for _ in range(num_rails)]
    row, direction = 0, 1
    for col in range(n):
        pattern[row][col] = '*'
        row += direction
        if row == 0 or row == num_rails - 1:
            direction *= -1
    index = 0
    for r in range(num_rails):
        for c in range(n):
            if pattern[r][c] == '*' and index < n:
                pattern[r][c] = cipher[index]
                index += 1
    result = []
    row, direction = 0, 1
    for col in range(n):
        result.append(pattern[row][col])
        row += direction
        if row == 0 or row == num_rails - 1:
            direction *= -1
    return ''.join(result)


def columnar_encrypt(text, key):
    text = text.replace(" ", "")
    num_cols = len(key)
    num_rows = -(-len(text) // num_cols)
    padded_length = num_rows * num_cols
    text = text.ljust(padded_length, 'X')
    matrix = []
    for i in range(num_rows):
        row = list(text[i * num_cols: (i + 1) * num_cols])
        matrix.append(row)
    key_order = sorted(list(enumerate(key)), key=lambda x: (x[1], x[0]))
    cipher = ''
    for col_index, _ in key_order:
        for row in matrix:
            cipher += row[col_index]
    return cipher


def columnar_decrypt(cipher, key):
    num_cols = len(key)
    num_rows = -(-len(cipher) // num_cols)
    key_order = sorted(list(enumerate(key)), key=lambda x: (x[1], x[0]))
    matrix = [[''] * num_cols for _ in range(num_rows)]
    index = 0
    for col_index, _ in key_order:
        for row in range(num_rows):
            matrix[row][col_index] = cipher[index]
            index += 1
    plaintext = ''
    for row in matrix:
        plaintext += ''.join(row)
    return plaintext


def combined_encrypt(text, num_rails, col_key):
    intermediate = rail_fence_encrypt(text, num_rails)
    final_cipher = columnar_encrypt(intermediate, col_key)
    return final_cipher


def combined_decrypt(cipher, num_rails, col_key):
    intermediate = columnar_decrypt(cipher, col_key)
    original_text = rail_fence_decrypt(intermediate, num_rails)
    return original_text


if __name__ == "__main__":
    plaintext = "WE ARE DISCOVERED FLEE AT ONCE"
    num_rails = 3
    rf_encrypted = rail_fence_encrypt(plaintext.replace(" ", ""), num_rails)
    rf_decrypted = rail_fence_decrypt(rf_encrypted, num_rails)
    print("=== Rail Fence Cipher ===")
    print("Plaintext: ", plaintext)
    print("Rail Fence Encrypted: ", rf_encrypted)
    print("Rail Fence Decrypted: ", rf_decrypted)
    print()
    col_key = "SECRET"
    col_encrypted = columnar_encrypt(plaintext, col_key)
    col_decrypted = columnar_decrypt(col_encrypted, col_key)
    print("=== Columnar Transposition Cipher ===")
    print("Plaintext: ", plaintext)
    print("Columnar Encrypted: ", col_encrypted)
    print("Columnar Decrypted (with padding): ", col_decrypted)
    print()
    combined = combined_encrypt(plaintext.replace(" ", ""), num_rails, col_key)
    recovered = combined_decrypt(combined, num_rails, col_key)
    print("=== Combined Cipher (Rail Fence then Columnar) ===")
    print("Plaintext: ", plaintext)
    print("Combined Encrypted: ", combined)
    print("Combined Decrypted: ", recovered)
