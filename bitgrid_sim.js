function simulate() {
    // Get input bits from checkboxes
    const inputBits = [
        document.getElementById('input0').checked ? 1 : 0,
        document.getElementById('input1').checked ? 1 : 0,
        document.getElementById('input2').checked ? 1 : 0,
        document.getElementById('input3').checked ? 1 : 0
    ];

    // Get lookup bits from checkboxes
    const lookupBits = [0, 1, 2, 3].map(index => {
        return Array.from({ length: 16 }, (_, i) => document.getElementById(`lookup${index}_${i}`).checked ? '1' : '0').join('');
    });

    // Validate lookup bits
    for (let i = 0; i < 4; i++) {
        if (lookupBits[i].length !== 16 || !/^[01]+$/.test(lookupBits[i])) {
            alert(`Lookup bits for input ${i} must be exactly 16 bits long and contain only 0s and 1s.`);
            return;
        }
    }

    // Calculate output bits
    const outputBits = inputBits.map((bit, index) => {
        const lookupIndex = parseInt(lookupBits[index], 2);
        return (lookupIndex >> bit) & 1;
    });

    // Update output display
    for (let i = 0; i < 4; i++) {
        document.getElementById(`output${i}`).innerText = outputBits[i];
    }
}