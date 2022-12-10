import argparse
import pathlib

def obj_to_str(wavefront_file) -> None:
    unique_string = ""
    lines = wavefront_file.read_text().splitlines()
    for line in lines:
        unique_string += f'"{line}\\n"\n'
    pathlib.Path(f"{wavefront_file.stem}.txt").write_text(f"{unique_string}")

def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--filename", type=str, required=True, help="Name of the Wavefront OBJ file to parse")
    args = parser.parse_args()
    obj_to_str(pathlib.Path(args.filename))

if __name__ == "__main__":
    main()