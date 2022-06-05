#pragma once

#include "svg.h"
#include "geo.h"
#include "transport_catalogue.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>

namespace renderer {

    inline const double EPSILON = 1e-6;
    inline bool IsZero(double value) {
        return std::abs(value) < EPSILON;
    }

    class SphereProjector {
    public:
        template <typename PointInputIt>
        SphereProjector(PointInputIt points_begin, PointInputIt points_end,
            double max_width, double max_height, double padding)
            : padding_(padding) //
        {

            if (points_begin == points_end) {
                return;
            }

            const auto [left_it, right_it] = std::minmax_element(
                points_begin, points_end,
                [](auto lhs, auto rhs) { return lhs.lng < rhs.lng; });
            min_lon_ = left_it->lng;
            const double max_lon = right_it->lng;

            const auto [bottom_it, top_it] = std::minmax_element(
                points_begin, points_end,
                [](auto lhs, auto rhs) { return lhs.lat < rhs.lat; });
            const double min_lat = bottom_it->lat;
            max_lat_ = top_it->lat;

            std::optional<double> width_zoom;
            if (!IsZero(max_lon - min_lon_)) {
                width_zoom = (max_width - 2 * padding) / (max_lon - min_lon_);
            }

            std::optional<double> height_zoom;
            if (!IsZero(max_lat_ - min_lat)) {
                height_zoom = (max_height - 2 * padding) / (max_lat_ - min_lat);
            }

            if (width_zoom && height_zoom) {
                zoom_coeff_ = (std::min) (*width_zoom, *height_zoom);
            }
            else if (width_zoom) {
                zoom_coeff_ = *width_zoom;
            }
            else if (height_zoom) {
                zoom_coeff_ = *height_zoom;
            }
        }

        svg::Point operator()(geo::Coordinates coords) const {
            return {
                (coords.lng - min_lon_) * zoom_coeff_ + padding_,
                (max_lat_ - coords.lat) * zoom_coeff_ + padding_
            };
        }

    private:
        double padding_;
        double min_lon_ = 0;
        double max_lat_ = 0;
        double zoom_coeff_ = 0;
    };

    using namespace std::literals;

    struct RenderSettings
    {
        double width;
        double height;
        double padding;
        double line_width;
        double stop_radius;
        int bus_label_font_size;
        svg::Point bus_label_offset;
        int stop_label_font_size;
        svg::Point stop_label_offset;
        svg::Color underlayer_color;
        double underlayer_width;
        std::vector<svg::Color> color_palette;
    };

	class MapRenderer {
        
	public:
        
        MapRenderer(const catalogue::TransportCatalogue& db, RenderSettings settings)
            :
            settings_(settings),
            db_(db)
        {
            const auto& buses = db_.GetBuses();
            for (const auto& bus : buses) {
                buses_name_view_.insert(bus.name);
            }

            for (const auto& bus_name : buses_name_view_) {
                const auto& bus = db_.FindBus(bus_name)->bus;
                for (const auto& stop : bus) {
                    stop_names_.insert(stop->name);
                }
            }
        }

        void SetRenderBus();

        void Print(std::ostream& out);

	private:
        void AddLinesToRenderer(const SphereProjector& proj);
        void AddBusNamesToRenderer(const SphereProjector& proj);
        void AddStopCirclesToRenderer(const SphereProjector& proj);
        void AddStopNamesToRenderer(const SphereProjector& proj);

        RenderSettings settings_;
        svg::Document render_bus_;
        const catalogue::TransportCatalogue& db_;
        std::set<std::string_view> buses_name_view_;
        std::set<std::string_view> stop_names_;
	};

} // renderer