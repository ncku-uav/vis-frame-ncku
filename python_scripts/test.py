from mavsdk import System
import asyncio

async def run():
    drone = System()
    drone.connect(system_address="udp://:14550")

if __name__ == "__main__":
    # Start the main function
    asyncio.ensure_future(run())

    # Runs the event loop until the program is canceled with e.g. CTRL-C
    asyncio.get_event_loop().run_forever()