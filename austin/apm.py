import ast
import sys
import time
from asyncio import sleep
from datetime import datetime

from austin import AsyncAustin
from austin.stats import parse_line
from pyfiglet import Figlet


# TODO: Refactor
class EndpointAST:
    def __init__(self, filename):
        self.tree = ast.parse(open(filename, "r").read(), filename)
        # Add parent information for navigating upward
        self.tree.parent = None
        for node in ast.walk(self.tree):
            for child in ast.iter_child_nodes(node):
                child.parent = node

    def search_for_endpoint(self, lineno):
        for node in ast.walk(self.tree):
            try:
                if node.lineno != lineno:
                    continue

                while node:
                    if isinstance(node, ast.FunctionDef) and node.name == "get":
                        p = node.parent
                        if p and isinstance(p, ast.ClassDef):
                            return f"{p.name} [GET]"
                    node = node.parent
            except AttributeError:
                # Not a useful node
                pass


def get_endpoint(event):
    thread, frames, duration = parse_line(event.encode())
    endpoint = None

    for i, frame in enumerate(frames):
        if frame[:4] == "get ":
            filename = frame[5:-1]
            # TODO: Cache parsed ASTs
            endpoint = EndpointAST(filename).search_for_endpoint(
                int(frames[i+1][1:])
            )

    return endpoint, thread


class Call:
    def __init__(self, endpoint, thread):
        self.endpoint = endpoint
        self.thread = thread
        self.start = time.time()
        self.end = self.start

    def stop(self):
        self.end = time.time()

    def duration(self):
        return self.end - self.start

    def __repr__(self):
        return f"Call to {self.endpoint} handled by {self.thread} ({self.duration():.2f}s)"


class CallRecorder:
    def __init__(self):
        self.endpoints = {}
        self.calls = {}
        self.threads = {}

    def handle_event(self, event):
        endpoint, thread = get_endpoint(event)

        if endpoint:
            if thread not in self.threads:
                self.threads[thread] = Call(endpoint, thread)
        else:
            try:
                call = self.threads[thread]
                call.stop()
                print(call)
                try:
                    self.calls[call.endpoint].append(call)
                except:
                    self.calls[call.endpoint] = [call]

                del self.threads[thread]
            except KeyError:
                pass


class AustinAPM(AsyncAustin):
    def on_sample_received(self, sample):
        self.recorder.handle_event(sample)

    def start(self, args):
        super().start(args)

        self.recorder = CallRecorder()

        if not self.wait(5):
            print("Austin took too long to start. Terminating...")
            exit(1)

        print(Figlet(font="speed", width=240).renderText("* Austin APM *"))
        print(
            f"* Sampling process with PID {self.get_pid()} "
            f"({self.get_cmd_line()})"
        )


def main():
    apm = AustinAPM()
    try:
        apm.start(sys.argv[1:])
        apm.join()
    except KeyboardInterrupt:
        pass
    finally:
        print("\n* Austin APM is shutting down.")


if __name__ == "__main__":
    main()
