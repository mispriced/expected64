import re
import sys

def parse_benchmark_output(output):
    # Updated regex pattern to capture different parts of the benchmark output
    # This pattern may need further refinement based on the actual format of your raw results
    test_pattern = r'-------------------------------------------------------------------------------\n(.*?)\n-------------------------------------------------------------------------------\n(.*?)\n==============================================================================='
    benchmark_pattern = r'(\w+ with [\w\:\:]+|Factorial with Raw Type|Cube with Raw Type)\s+(\d+)\s+(\d+)\s+([\d\.]+ ms)\s+([\d\.]+ ns)\s+([\d\.]+ ns)\s+([\d\.]+ ns)\s+([\d\.]+ ns)\s+([\d\.]+ ns)\s+([\d\.]+ ns)'

    tests = re.findall(test_pattern, output, re.DOTALL)
    parsed_data = {}

    for test_name, test_content in tests:
        benchmarks = re.findall(benchmark_pattern, test_content)
        parsed_data[test_name.strip()] = benchmarks

    return parsed_data

def generate_html_tables(data):
    html = ''
    for test_name, benchmarks in data.items():
        html += '<table border="1">\n'
        html += '<tr><th>Benchmark Name</th><th>Samples</th><th>Iterations</th><th>Estimated</th><th>Mean</th><th>Low Mean</th><th>High Mean</th><th>Std Dev</th><th>Low Std Dev</th><th>High Std Dev</th></tr>\n'
        for benchmark in benchmarks:
            html += '<tr>' + ''.join(f'<td>{cell}</td>' for cell in benchmark) + '</tr>\n'
        html += '</table>\n'
    return html

def main():
    if len(sys.argv) != 2:
        print("Usage: python benchmark_parser.py [benchmark_output_file]")
        sys.exit(1)

    benchmark_file = sys.argv[1]

    try:
        with open(benchmark_file, 'r') as file:
            benchmark_output = file.read()
        
        parsed_data = parse_benchmark_output(benchmark_output)
        html_tables = generate_html_tables(parsed_data)
        print(html_tables)

    except FileNotFoundError:
        print(f"Error: File '{benchmark_file}' not found.")
        sys.exit(1)

if __name__ == "__main__":
    main()
