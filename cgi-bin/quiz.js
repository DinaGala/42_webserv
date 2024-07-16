const readline = require('readline');
const fs = require('fs');

// Questions for the quiz
const questions = [
    "What is your name?",
    "What is your favorite programming language?",
    "What is your favorite hobby?"
];

// Create an interface to read lines from the console
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

let answers = [];
let currentQuestion = 0;

// Function to ask the next question
const askQuestion = () => {
    if (currentQuestion < questions.length) {
        rl.question(questions[currentQuestion] + " ", (answer) => {
            answers.push(answer);
            currentQuestion++;
            askQuestion();
        });
    } else {
        // Generate a unique file name using the current timestamp
        const timestamp = new Date().toISOString().replace(/:/g, '-');
        const fileName = `quiz_${timestamp}.txt`;

        // When all questions are answered, write answers to the file
        const fileContent = answers.map((answer, index) => `Q: ${questions[index]}\nA: ${answer}`).join('\n\n');
        fs.writeFile(fileName, fileContent, (err) => {
            if (err) throw err;
            console.log(`Your answers have been saved to ${fileName}`);
            rl.close();
        });
    }
};

// Start the quiz
askQuestion();
