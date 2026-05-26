import dearpygui.dearpygui as dpg

def save_callback():
    print("Save Clicked")

def create():
    dpg.create_context()
    dpg.create_viewport()
    dpg.setup_dearpygui()

def destroy():
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()


def main():
    create()

    with dpg.window(label="Example Window"):
        dpg.add_text("Hello world")
        dpg.add_button(label="Save", callback=save_callback)
        dpg.add_input_text(label="string")
        dpg.add_slider_float(label="float")

    destroy()

main()