import gradio as gr
import subprocess

def run_compiler(code):
    code = code.strip()

    if not code.endswith(";"):
        code += ";"

    with open("test.hxi", "w") as f:
        f.write(code)

    compile_result = subprocess.run(
        ["gcc", "compiler.c", "-o", "helix"],
        capture_output=True,
        text=True
    )

    if compile_result.returncode != 0:
        return "Compilation failed:\n\n" + compile_result.stderr

    result = subprocess.run(
        ["./helix", "test.hxi"],
        capture_output=True,
        text=True
    )

    return result.stdout if result.stdout else result.stderr

demo = gr.Interface(
    fn=run_compiler,
    inputs=gr.Textbox(
        label="Enter Expression",
        placeholder="Example: (8+2)/5 or 9-3+2*4",
        lines=2
    ),
    outputs=gr.Textbox(label="Compiler Output", lines=22),
    title="Helix Compiler",
    description="C-based compiler with Lexer, Parser, AST, TAC, and DAG optimization."
)

demo.launch()