// midi_pybind.cpp

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "midi_writer.h"

namespace py = pybind11;

PYBIND11_MODULE(midiwriter, m) {
    m.doc() = "Python bindings for MidiWriter using pybind11";

    py::class_<MidiWriter>(m, "MidiWriter")
        .def(py::init<>())
        .def("add_track", &MidiWriter::add_track, "Add a new track and return its index")
        .def("set_channel", &MidiWriter::set_channel,
             py::arg("channel"), 
             py::arg("program"),
             "Set the instrument program for a given channel")
        .def("add_bpm", &MidiWriter::add_bpm,
             py::arg("track_index"), 
             py::arg("start"), 
             py::arg("bpm"),
             "Add a tempo change (beats per minute)")
        .def("add_note", &MidiWriter::add_note,
             py::arg("track_index"), 
             py::arg("channel"),
             py::arg("start"), 
             py::arg("duration"),
             py::arg("pitch"), py::arg("velocity"),
             py::arg("off_velocity") = 64,
             "Add a note event")
        .def("add_time_signature", &MidiWriter::add_time_signature,
             py::arg("track_index"), 
             py::arg("start"),
             py::arg("numerator"), 
             py::arg("denominator"),
             "Add a time signature event")
        .def("add_track_name", &MidiWriter::add_track_name,
             py::arg("track_index"), 
             py::arg("name"),
             py::arg("start") = 0,
             "Add a track name event")
        .def("save", &MidiWriter::save,
             py::arg("filename"),
             "Write the MIDI file to disk");
}
