#define VIDEO_MEMORY 0xB8000
#define MAX_INPUT_LENGTH 100


void print_string(const char *str) {
    char *video_memory = (char *)VIDEO_MEMORY;
    int i = 0;
    while (str[i] != '\0') {
        video_memory[i * 2] = str[i];      
        video_memory[i * 2 + 1] = 0x07;    
        i++;
    }
}


void clear_screen() {
    char *video_memory = (char *)VIDEO_MEMORY;
    for (int i = 0; i < 80 * 25; i++) {
        video_memory[i * 2] = ' ';   
        video_memory[i * 2 + 1] = 0x07; 
    }
}


void get_input(char *input) {
    char *video_memory = (char *)VIDEO_MEMORY;
    int i = 0;
    char ch;


    while (1) {
        ch = __inb(0x60);  
        if (ch == '\n' || ch == '\r') {
            input[i] = '\0';  
            break;
        } else if (ch == 0x08) {  
            if (i > 0) {
                i--;
                video_memory[(i) * 2] = ' ';  
            }
        } else {
            input[i] = ch;
            video_memory[(i) * 2] = ch;  
            video_memory[(i) * 2 + 1] = 0x07;
            i++;
        }
    }
}


void itoa(int num, char *str, int base) {
    int i = 0;
    int is_negative = 0;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0 && base == 10) {
        is_negative = 1;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}


int calculate(int num1, int num2, char op) {
    int result = 0;
    switch (op) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 != 0) {
                result = num1 / num2;
            } else {
                print_string("Error: Division by zero.\n");
                return -1;  // Bölme hatası
            }
            break;
        default:
            print_string("Invalid operator.\n");
            return -1;
    }
    return result;
}


void run_calculator() {
    print_string("Simple Calculator\n");
    print_string("Enter first number: ");
    char num1_str[MAX_INPUT_LENGTH];
    get_input(num1_str);
    int num1 = 0;
    for (int i = 0; num1_str[i] != '\0'; i++) {
        num1 = num1 * 10 + (num1_str[i] - '0');
    }

    print_string("Enter operator (+, -, *, /): ");
    char op;
    op = __inb(0x60);  // Bir karakter oku (operatör)

    print_string("Enter second number: ");
    char num2_str[MAX_INPUT_LENGTH];
    get_input(num2_str);
    int num2 = 0;
    for (int i = 0; num2_str[i] != '\0'; i++) {
        num2 = num2 * 10 + (num2_str[i] - '0');
    }

    int result = calculate(num1, num2, op);
    if (result != -1) {
        print_string("Result: ");
        char result_str[20];
        itoa(result, result_str, 10);  // Sayıyı string'e dönüştür
        print_string(result_str);
        print_string("\n");
    }
}


void kernel_main() {
    const char *welcome_message = "basic kernel !\n";
    print_string(welcome_message);

    while (1) {
        clear_screen();  
        print_string("Choose an option:\n");
        print_string("1. Run Calculator\n");
        print_string("2. Exit\n");

        char choice = __inb(0x60);  

        if (choice == '1') {
            run_calculator();  
        } else if (choice == '2') {
            break;  
        } else {
            print_string("Invalid option.\n");
        }
    }
}
