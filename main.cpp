#include <windows.h>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

#define MAIN_DISPLAY 100
#define SCI_DISPLAY  200

// Main calculator buttons
#define M0 1
#define M1 2
#define M2 3
#define M3 4
#define M4 5
#define M5 6
#define M6 7
#define M7 8
#define M8 9
#define M9 10

#define M_ADD      11
#define M_SUB      12
#define M_MUL      13
#define M_DIV      14
#define M_EQUAL    15
#define M_CLEAR    16
#define M_DOT      17
#define M_PERCENT  18
#define M_SCI      19

// Scientific numeric buttons
#define S0 30
#define S1 31
#define S2 32
#define S3 33
#define S4 34
#define S5 35
#define S6 36
#define S7 37
#define S8 38
#define S9 39

#define S_ADD      40
#define S_SUB      41
#define S_MUL      42
#define S_DIV      43
#define S_EQUAL    44
#define S_CLEAR    45
#define S_DOT      46

// Scientific functions
#define S_SIN      50
#define S_COS      51
#define S_TAN      52
#define S_CSC      53
#define S_SEC      54
#define S_COT      55

#define S_LOG      56
#define S_LN       57
#define S_LOG2     58

#define S_SQRT     59
#define S_SQUARE   60
#define S_POWER    61
#define S_FACTORIAL 62

#define S_ENTROPY  63
#define S_PI       64
#define S_E        65

#define S_BACK     66


// ---------------------------------------------------------
// GLOBAL VARIABLES
// ---------------------------------------------------------

HWND mainDisplay;
HWND sciDisplay;

HWND mainWindow;
HWND scientificWindow;

double mainFirstNumber = 0;
char mainOperation = 0;
bool mainNewNumber = true;

double sciFirstNumber = 0;
char sciOperation = 0;
bool sciNewNumber = true;

bool powerMode = false;

HFONT normalFont;
HFONT displayFont;


// ---------------------------------------------------------
// NUMBER FORMAT
// ---------------------------------------------------------

std::string formatNumber(double value)
{
    std::ostringstream ss;

    ss << std::setprecision(12) << value;

    std::string result = ss.str();

    if (result.find('.') != std::string::npos)
    {
        while (!result.empty() && result.back() == '0')
            result.pop_back();

        if (!result.empty() && result.back() == '.')
            result.pop_back();
    }

    if (result == "-0")
        result = "0";

    return result;
}


// ---------------------------------------------------------
// MAIN DISPLAY
// ---------------------------------------------------------

double getMainNumber()
{
    char text[200];

    GetWindowTextA(
        mainDisplay,
        text,
        sizeof(text)
    );

    try
    {
        return std::stod(text);
    }
    catch (...)
    {
        return 0;
    }
}

void setMainNumber(double number)
{
    std::string text = formatNumber(number);

    SetWindowTextA(
        mainDisplay,
        text.c_str()
    );
}


// ---------------------------------------------------------
// SCIENTIFIC DISPLAY
// ---------------------------------------------------------

double getSciNumber()
{
    char text[200];

    GetWindowTextA(
        sciDisplay,
        text,
        sizeof(text)
    );

    try
    {
        return std::stod(text);
    }
    catch (...)
    {
        return 0;
    }
}

void setSciNumber(double number)
{
    std::string text = formatNumber(number);

    SetWindowTextA(
        sciDisplay,
        text.c_str()
    );
}


// ---------------------------------------------------------
// MAIN CALCULATOR
// ---------------------------------------------------------

void mainAddDigit(char digit)
{
    if (mainNewNumber)
    {
        std::string text(1, digit);

        SetWindowTextA(
            mainDisplay,
            text.c_str()
        );

        mainNewNumber = false;
        return;
    }

    char buffer[200];

    GetWindowTextA(
        mainDisplay,
        buffer,
        sizeof(buffer)
    );

    std::string text(buffer);

    if (text == "0")
        text = digit;
    else
        text += digit;

    SetWindowTextA(
        mainDisplay,
        text.c_str()
    );
}

void mainDecimal()
{
    if (mainNewNumber)
    {
        SetWindowTextA(
            mainDisplay,
            "0."
        );

        mainNewNumber = false;
        return;
    }

    char buffer[200];

    GetWindowTextA(
        mainDisplay,
        buffer,
        sizeof(buffer)
    );

    std::string text(buffer);

    if (text.find('.') == std::string::npos)
    {
        text += ".";

        SetWindowTextA(
            mainDisplay,
            text.c_str()
        );
    }
}

void mainChooseOperation(char op)
{
    mainFirstNumber = getMainNumber();

    mainOperation = op;

    mainNewNumber = true;
}

void mainCalculate()
{
    double secondNumber = getMainNumber();

    double result = 0;

    switch (mainOperation)
    {
        case '+':
            result = mainFirstNumber + secondNumber;
            break;

        case '-':
            result = mainFirstNumber - secondNumber;
            break;

        case '*':
            result = mainFirstNumber * secondNumber;
            break;

        case '/':
            if (secondNumber == 0)
            {
                SetWindowTextA(
                    mainDisplay,
                    "Error"
                );

                mainNewNumber = true;
                return;
            }

            result = mainFirstNumber / secondNumber;
            break;

        default:
            return;
    }

    setMainNumber(result);

    mainOperation = 0;
    mainNewNumber = true;
}

void mainClear()
{
    SetWindowTextA(
        mainDisplay,
        "0"
    );

    mainFirstNumber = 0;
    mainOperation = 0;
    mainNewNumber = true;
}

void mainPercent()
{
    double number = getMainNumber();

    setMainNumber(
        number / 100.0
    );

    mainNewNumber = true;
}


// ---------------------------------------------------------
// SCIENTIFIC NUMBERS
// ---------------------------------------------------------

void sciAddDigit(char digit)
{
    if (sciNewNumber)
    {
        std::string text(1, digit);

        SetWindowTextA(
            sciDisplay,
            text.c_str()
        );

        sciNewNumber = false;
        return;
    }

    char buffer[200];

    GetWindowTextA(
        sciDisplay,
        buffer,
        sizeof(buffer)
    );

    std::string text(buffer);

    if (text == "0")
        text = digit;
    else
        text += digit;

    SetWindowTextA(
        sciDisplay,
        text.c_str()
    );
}

void sciDecimal()
{
    if (sciNewNumber)
    {
        SetWindowTextA(
            sciDisplay,
            "0."
        );

        sciNewNumber = false;
        return;
    }

    char buffer[200];

    GetWindowTextA(
        sciDisplay,
        buffer,
        sizeof(buffer)
    );

    std::string text(buffer);

    if (text.find('.') == std::string::npos)
    {
        text += ".";

        SetWindowTextA(
            sciDisplay,
            text.c_str()
        );
    }
}


// ---------------------------------------------------------
// SCIENTIFIC BASIC OPERATIONS
// ---------------------------------------------------------

void sciChooseOperation(char op)
{
    sciFirstNumber = getSciNumber();

    sciOperation = op;

    sciNewNumber = true;
}

void sciCalculate()
{
    double secondNumber = getSciNumber();

    double result = 0;

    switch (sciOperation)
    {
        case '+':
            result = sciFirstNumber + secondNumber;
            break;

        case '-':
            result = sciFirstNumber - secondNumber;
            break;

        case '*':
            result = sciFirstNumber * secondNumber;
            break;

        case '/':
            if (secondNumber == 0)
            {
                SetWindowTextA(
                    sciDisplay,
                    "Error"
                );

                sciNewNumber = true;
                return;
            }

            result = sciFirstNumber / secondNumber;
            break;

        default:
            return;
    }

    setSciNumber(result);

    sciOperation = 0;
    sciNewNumber = true;
}


// ---------------------------------------------------------
// FACTORIAL
// ---------------------------------------------------------

double factorial(double n)
{
    if (n < 0 || floor(n) != n || n > 170)
        return -1;

    double result = 1;

    for (int i = 1; i <= (int)n; i++)
        result *= i;

    return result;
}


// ---------------------------------------------------------
// SCIENTIFIC FUNCTIONS
// ---------------------------------------------------------

void scientificFunction(int id)
{
    double x = getSciNumber();

    double result = 0;

    const double PI = 3.14159265358979323846;

    if (id == S_SIN)
    {
        result = sin(x * PI / 180.0);
    }

    else if (id == S_COS)
    {
        result = cos(x * PI / 180.0);
    }

    else if (id == S_TAN)
    {
        double angle = x * PI / 180.0;

        if (fabs(cos(angle)) < 1e-12)
        {
            SetWindowTextA(
                sciDisplay,
                "Error"
            );

            sciNewNumber = true;
            return;
        }

        result = tan(angle);
    }

    else if (id == S_CSC)
    {
        double angle = x * PI / 180.0;

        double s = sin(angle);

        if (fabs(s) < 1e-12)
        {
            SetWindowTextA(
                sciDisplay,
                "Error"
            );

            sciNewNumber = true;
            return;
        }

        result = 1.0 / s;
    }

    else if (id == S_SEC)
    {
        double angle = x * PI / 180.0;

        double c = cos(angle);

        if (fabs(c) < 1e-12)
        {
            SetWindowTextA(
                sciDisplay,
                "Error"
            );

            sciNewNumber = true;
            return;
        }

        result = 1.0 / c;
    }

    else if (id == S_COT)
    {
        double angle = x * PI / 180.0;

        double s = sin(angle);

        if (fabs(s) < 1e-12)
        {
            SetWindowTextA(
                sciDisplay,
                "Error"
            );

            sciNewNumber = true;
            return;
        }

        result = cos(angle) / s;
    }

    // LOG BASE 10
    else if (id == S_LOG)
    {
        if (x <= 0)
        {
            SetWindowTextA(
                sciDisplay,
                "Error"
            );

            sciNewNumber = true;
            return;
        }

        result = log10(x);
    }

    // LOG BASE e
    else if (id == S_LN)
    {
        if (x <= 0)
        {
            SetWindowTextA(
                sciDisplay,
                "Error"
            );

            sciNewNumber = true;
            return;
        }

        result = log(x);
    }

    // LOG BASE 2
    else if (id == S_LOG2)
    {
        if (x <= 0)
        {
            SetWindowTextA(
                sciDisplay,
                "Error"
            );

            sciNewNumber = true;
            return;
        }

        result = log2(x);
    }

    // SQUARE ROOT
    else if (id == S_SQRT)
    {
        if (x < 0)
        {
            SetWindowTextA(
                sciDisplay,
                "Error"
            );

            sciNewNumber = true;
            return;
        }

        result = sqrt(x);
    }

    // SQUARE
    else if (id == S_SQUARE)
    {
        result = x * x;
    }

    // FACTORIAL
    else if (id == S_FACTORIAL)
    {
        result = factorial(x);

        if (result < 0)
        {
            SetWindowTextA(
                sciDisplay,
                "Error"
            );

            sciNewNumber = true;
            return;
        }
    }

    // PI
    else if (id == S_PI)
    {
        result = 3.14159265358979323846;
    }

    // e
    else if (id == S_E)
    {
        result = 2.71828182845904523536;
    }

    // ENTROPY
    else if (id == S_ENTROPY)
    {
        /*
            Binary Shannon Entropy:

            H(p) =
            -p log2(p)
            -(1-p) log2(1-p)

            Enter p between 0 and 1.
            Example:
            p = 0.5
            H(0.5) = 1
        */

        if (x < 0 || x > 1)
        {
            SetWindowTextA(
                sciDisplay,
                "Enter p: 0 to 1"
            );

            sciNewNumber = true;
            return;
        }

        if (x == 0 || x == 1)
        {
            result = 0;
        }
        else
        {
            result =
                -x * log2(x)
                -(1 - x) * log2(1 - x);
        }
    }

    setSciNumber(result);

    sciNewNumber = true;
}


// ---------------------------------------------------------
// POWER x^y
// ---------------------------------------------------------

void startPower()
{
    sciFirstNumber = getSciNumber();

    powerMode = true;

    SetWindowTextA(
        sciDisplay,
        "0"
    );

    sciNewNumber = false;
}

void calculatePower()
{
    if (!powerMode)
        return;

    double exponent = getSciNumber();

    double result = pow(
        sciFirstNumber,
        exponent
    );

    if (std::isnan(result) || std::isinf(result))
    {
        SetWindowTextA(
            sciDisplay,
            "Error"
        );
    }
    else
    {
        setSciNumber(result);
    }

    powerMode = false;
    sciNewNumber = true;
}


// ---------------------------------------------------------
// CREATE BUTTON
// ---------------------------------------------------------

void createButton(
    HWND parent,
    const char* text,
    int id,
    int x,
    int y,
    int width,
    int height)
{
    HWND button = CreateWindowA(
        "BUTTON",
        text,
        WS_VISIBLE |
        WS_CHILD |
        BS_PUSHBUTTON,
        x,
        y,
        width,
        height,
        parent,
        (HMENU)(INT_PTR)id,
        GetModuleHandle(NULL),
        NULL
    );

    SendMessage(
        button,
        WM_SETFONT,
        (WPARAM)normalFont,
        TRUE
    );
}


// ---------------------------------------------------------
// MAIN WINDOW
// ---------------------------------------------------------

void createMainCalculator(HWND hwnd)
{
    int w = 75;
    int h = 55;

    // Row 1
    createButton(hwnd, "7", M7, 25, 130, w, h);
    createButton(hwnd, "8", M8, 110, 130, w, h);
    createButton(hwnd, "9", M9, 195, 130, w, h);
    createButton(hwnd, "/", M_DIV, 280, 130, w, h);

    // Row 2
    createButton(hwnd, "4", M4, 25, 195, w, h);
    createButton(hwnd, "5", M5, 110, 195, w, h);
    createButton(hwnd, "6", M6, 195, 195, w, h);
    createButton(hwnd, "*", M_MUL, 280, 195, w, h);

    // Row 3
    createButton(hwnd, "1", M1, 25, 260, w, h);
    createButton(hwnd, "2", M2, 110, 260, w, h);
    createButton(hwnd, "3", M3, 195, 260, w, h);
    createButton(hwnd, "-", M_SUB, 280, 260, w, h);

    // Row 4
    createButton(hwnd, "0", M0, 25, 325, w, h);
    createButton(hwnd, ".", M_DOT, 110, 325, w, h);
    createButton(hwnd, "%", M_PERCENT, 195, 325, w, h);
    createButton(hwnd, "+", M_ADD, 280, 325, w, h);

    // Row 5
    createButton(
        hwnd,
        "CLEAR",
        M_CLEAR,
        25,
        390,
        160,
        h
    );

    createButton(
        hwnd,
        "=",
        M_EQUAL,
        195,
        390,
        160,
        h
    );

    // Scientific button
    createButton(
        hwnd,
        "SCIENTIFIC",
        M_SCI,
        25,
        465,
        330,
        60
    );
}


// ---------------------------------------------------------
// SCIENTIFIC WINDOW
// ---------------------------------------------------------

void createScientificCalculator(HWND hwnd)
{
    // Scientific display
    sciDisplay = CreateWindowA(
        "EDIT",
        "0",
        WS_VISIBLE |
        WS_CHILD |
        WS_BORDER |
        ES_RIGHT |
        ES_AUTOHSCROLL,
        25,
        25,
        435,
        65,
        hwnd,
        (HMENU)SCI_DISPLAY,
        GetModuleHandle(NULL),
        NULL
    );

    SendMessage(
        sciDisplay,
        WM_SETFONT,
        (WPARAM)displayFont,
        TRUE
    );

    int w = 75;
    int h = 48;

    // -------------------------------
    // Scientific functions
    // -------------------------------

    createButton(hwnd, "sin", S_SIN, 25, 110, w, h);
    createButton(hwnd, "cos", S_COS, 110, 110, w, h);
    createButton(hwnd, "tan", S_TAN, 195, 110, w, h);
    createButton(hwnd, "cosec", S_CSC, 280, 110, w, h);
    createButton(hwnd, "sec", S_SEC, 365, 110, w, h);

    createButton(hwnd, "cot", S_COT, 25, 165, w, h);
    createButton(hwnd, "log", S_LOG, 110, 165, w, h);
    createButton(hwnd, "ln", S_LN, 195, 165, w, h);
    createButton(hwnd, "log2", S_LOG2, 280, 165, w, h);
    createButton(hwnd, "sqrt", S_SQRT, 365, 165, w, h);

    createButton(hwnd, "x^2", S_SQUARE, 25, 220, w, h);
    createButton(hwnd, "x^y", S_POWER, 110, 220, w, h);
    createButton(hwnd, "x!", S_FACTORIAL, 195, 220, w, h);
    createButton(hwnd, "H(p)", S_ENTROPY, 280, 220, w, h);
    createButton(hwnd, "pi", S_PI, 365, 220, w, h);

    // -------------------------------
    // Numbers
    // -------------------------------

    createButton(hwnd, "7", S7, 25, 290, w, h);
    createButton(hwnd, "8", S8, 110, 290, w, h);
    createButton(hwnd, "9", S9, 195, 290, w, h);
    createButton(hwnd, "/", S_DIV, 280, 290, w, h);
    createButton(hwnd, "e", S_E, 365, 290, w, h);

    createButton(hwnd, "4", S4, 25, 345, w, h);
    createButton(hwnd, "5", S5, 110, 345, w, h);
    createButton(hwnd, "6", S6, 195, 345, w, h);
    createButton(hwnd, "*", S_MUL, 280, 345, w, h);
    createButton(hwnd, "C", S_CLEAR, 365, 345, w, h);

    createButton(hwnd, "1", S1, 25, 400, w, h);
    createButton(hwnd, "2", S2, 110, 400, w, h);
    createButton(hwnd, "3", S3, 195, 400, w, h);
    createButton(hwnd, "-", S_SUB, 280, 400, w, h);
    createButton(hwnd, "BACK", S_BACK, 365, 400, w, h);

    createButton(hwnd, "0", S0, 25, 455, w, h);
    createButton(hwnd, ".", S_DOT, 110, 455, w, h);
    createButton(hwnd, "+", S_ADD, 195, 455, w, h);

    createButton(
        hwnd,
        "=",
        S_EQUAL,
        280,
        455,
        160,
        h
    );
}


// ---------------------------------------------------------
// MAIN WINDOW PROCEDURE
// ---------------------------------------------------------

LRESULT CALLBACK MainWindowProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (message)
    {
        case WM_COMMAND:
        {
            int id = LOWORD(wParam);

            if (id >= M0 && id <= M9)
            {
                char digit =
                    '0' + (id - M0);

                mainAddDigit(digit);
            }

            else if (id == M_DOT)
            {
                mainDecimal();
            }

            else if (id == M_ADD)
            {
                mainChooseOperation('+');
            }

            else if (id == M_SUB)
            {
                mainChooseOperation('-');
            }

            else if (id == M_MUL)
            {
                mainChooseOperation('*');
            }

            else if (id == M_DIV)
            {
                mainChooseOperation('/');
            }

            else if (id == M_EQUAL)
            {
                mainCalculate();
            }

            else if (id == M_CLEAR)
            {
                mainClear();
            }

            else if (id == M_PERCENT)
            {
                mainPercent();
            }

            else if (id == M_SCI)
            {
                // Open scientific calculator
                if (!scientificWindow)
                {
                    scientificWindow = CreateWindowA(
                        "ScientificWindow",
                        "Calcii - Scientific Calculator",
                        WS_OVERLAPPEDWINDOW,
                        850,
                        100,
                        510,
                        590,
                        NULL,
                        NULL,
                        GetModuleHandle(NULL),
                        NULL
                    );

                    createScientificCalculator(
                        scientificWindow
                    );
                }

                ShowWindow(
                    scientificWindow,
                    SW_SHOW
                );

                SetForegroundWindow(
                    scientificWindow
                );
            }

            return 0;
        }

        case WM_DESTROY:

            PostQuitMessage(0);

            return 0;
    }

    return DefWindowProcA(
        hwnd,
        message,
        wParam,
        lParam
    );
}


// ---------------------------------------------------------
// SCIENTIFIC WINDOW PROCEDURE
// ---------------------------------------------------------

LRESULT CALLBACK ScientificWindowProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (message)
    {
        case WM_COMMAND:
        {
            int id = LOWORD(wParam);

            // Numbers
            if (id >= S0 && id <= S9)
            {
                char digit =
                    '0' + (id - S0);

                sciAddDigit(digit);
            }

            // Decimal
            else if (id == S_DOT)
            {
                sciDecimal();
            }

            // Basic operations
            else if (id == S_ADD)
            {
                sciChooseOperation('+');
            }

            else if (id == S_SUB)
            {
                sciChooseOperation('-');
            }

            else if (id == S_MUL)
            {
                sciChooseOperation('*');
            }

            else if (id == S_DIV)
            {
                sciChooseOperation('/');
            }

            else if (id == S_EQUAL)
            {
                if (powerMode)
                    calculatePower();
                else
                    sciCalculate();
            }

            // Clear
            else if (id == S_CLEAR)
            {
                SetWindowTextA(
                    sciDisplay,
                    "0"
                );

                sciFirstNumber = 0;
                sciOperation = 0;
                sciNewNumber = true;
                powerMode = false;
            }

            // Backspace
            else if (id == S_BACK)
            {
                char buffer[200];

                GetWindowTextA(
                    sciDisplay,
                    buffer,
                    sizeof(buffer)
                );

                std::string text(buffer);

                if (text.length() > 1)
                {
                    text.pop_back();

                    SetWindowTextA(
                        sciDisplay,
                        text.c_str()
                    );
                }
                else
                {
                    SetWindowTextA(
                        sciDisplay,
                        "0"
                    );
                }
            }

            // x^y
            else if (id == S_POWER)
            {
                startPower();
            }

            // Scientific functions
            else if (
                id >= S_SIN &&
                id <= S_E
            )
            {
                scientificFunction(id);
            }

            return 0;
        }

        case WM_CLOSE:

            ShowWindow(
                hwnd,
                SW_HIDE
            );

            return 0;
    }

    return DefWindowProcA(
        hwnd,
        message,
        wParam,
        lParam
    );
}


// ---------------------------------------------------------
// PROGRAM START
// ---------------------------------------------------------

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    LPSTR,
    int nCmdShow)
{
    // Fonts
    normalFont = CreateFontA(
        18,
        0,
        0,
        0,
        FW_NORMAL,
        FALSE,
        FALSE,
        FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH,
        "Segoe UI"
    );

    displayFont = CreateFontA(
        30,
        0,
        0,
        0,
        FW_BOLD,
        FALSE,
        FALSE,
        FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH,
        "Segoe UI"
    );


    // -----------------------------------------------------
    // Main window class
    // -----------------------------------------------------

    WNDCLASSA mainClass = {};

    mainClass.lpfnWndProc =
        MainWindowProc;

    mainClass.hInstance =
        hInstance;

    mainClass.lpszClassName =
        "CalciiMainWindow";

    mainClass.hbrBackground =
        (HBRUSH)(COLOR_WINDOW + 1);

    mainClass.hCursor =
        LoadCursor(
            NULL,
            IDC_ARROW
        );

    RegisterClassA(&mainClass);


    // -----------------------------------------------------
    // Scientific window class
    // -----------------------------------------------------

    WNDCLASSA sciClass = {};

    sciClass.lpfnWndProc =
        ScientificWindowProc;

    sciClass.hInstance =
        hInstance;

    sciClass.lpszClassName =
        "ScientificWindow";

    sciClass.hbrBackground =
        (HBRUSH)(COLOR_WINDOW + 1);

    sciClass.hCursor =
        LoadCursor(
            NULL,
            IDC_ARROW
        );

    RegisterClassA(&sciClass);


    // -----------------------------------------------------
    // Create main calculator
    // -----------------------------------------------------

    mainWindow = CreateWindowA(
        "CalciiMainWindow",
        "Calcii - Akshat Yadav",
        WS_OVERLAPPEDWINDOW,
        350,
        100,
        410,
        600,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!mainWindow)
        return 0;


    // Main display
    mainDisplay = CreateWindowA(
        "EDIT",
        "0",
        WS_VISIBLE |
        WS_CHILD |
        WS_BORDER |
        ES_RIGHT |
        ES_AUTOHSCROLL,
        25,
        30,
        330,
        70,
        mainWindow,
        (HMENU)MAIN_DISPLAY,
        hInstance,
        NULL
    );

    SendMessage(
        mainDisplay,
        WM_SETFONT,
        (WPARAM)displayFont,
        TRUE
    );


    createMainCalculator(
        mainWindow
    );


    ShowWindow(
        mainWindow,
        nCmdShow
    );

    UpdateWindow(
        mainWindow
    );


    // -----------------------------------------------------
    // Message loop
    // -----------------------------------------------------

    MSG msg = {};

    while (
        GetMessage(
            &msg,
            NULL,
            0,
            0
        )
    )
    {
        TranslateMessage(&msg);

        DispatchMessage(&msg);
    }


    return 0;
}